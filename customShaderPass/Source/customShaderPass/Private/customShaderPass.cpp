// Copyright Epic Games, Inc. All Rights Reserved.

#include "customShaderPass.h"
#include "ShaderCore.h"
#include "Misc/Paths.h"
#include "CommonRenderResources.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FcustomShaderPassModule"

void FcustomShaderPassModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// 最开始的时候将插件目录添加进去
	
	
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("customShaderPass"))->GetBaseDir(),TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/customShaderPass"),PluginShaderDir);
	
	
}

void FcustomShaderPassModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

void FcustomShaderPassModule::RenderCustomPass(UTextureRenderTarget2D* OutputRenderTarget)
{
	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
	ENQUEUE_RENDER_COMMAND(CaptureCommand)([this, TextureRenderTargetResource](FRHICommandListImmediate& RHICmdList)
		{
			RenderCustomPass(TextureRenderTargetResource, RHICmdList);
		});
}

void FcustomShaderPassModule::RenderCustomPass(FTextureRenderTargetResource* OutputRenderTarget, FRHICommandListImmediate& RHICmdList)
{

	check(IsInRenderingThread());

	FRHITexture2D* RenderTargetTexture = OutputRenderTarget->GetRenderTargetTexture();
	RHICmdList.Transition(FRHITransitionInfo(RenderTargetTexture,ERHIAccess::SRVMask,ERHIAccess::RTV));

	FRHIRenderPassInfo RPInfo(RenderTargetTexture,ERenderTargetActions::DontLoad_DontStore);
	RHICmdList.BeginRenderPass(RPInfo,TEXT("RenderPassHello"));
	{
		FGlobalShaderMap* shaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
		
		TShaderMapRef<FCustomVSShader> customVSShader(shaderMap);
		TShaderMapRef<FCustomPSShader> customPSShader(shaderMap);

		//初始化PSO
		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

		GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false,CF_Always>::GetRHI();
		
		GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
		GraphicsPSOInit.BoundShaderState.VertexShaderRHI = customVSShader.GetVertexShader();
		GraphicsPSOInit.BoundShaderState.PixelShaderRHI = customPSShader.GetPixelShader();

		GraphicsPSOInit.PrimitiveType = PT_TriangleList;

		SetGraphicsPipelineState(RHICmdList,GraphicsPSOInit,0);
		RHICmdList.SetViewport(0,0,0.0f,200,200,1.0f);

		RHICmdList.DrawPrimitive(0,2,1);
	}


	RHICmdList.EndRenderPass();
	RHICmdList.Transition(FRHITransitionInfo(RenderTargetTexture, ERHIAccess::RTV, ERHIAccess::SRVMask));

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FcustomShaderPassModule, customShaderPass)