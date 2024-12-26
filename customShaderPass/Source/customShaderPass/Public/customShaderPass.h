// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CustomShader.h"




//¼ÓÔØShaderÎÄ¼þ
IMPLEMENT_SHADER_TYPE(, FCustomPSShader, TEXT("/Plugin/customShaderPass/CustomShader.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FCustomVSShader, TEXT("/Plugin/customShaderPass/CustomShader.usf"), TEXT("MainVS"), SF_Vertex);

class CUSTOMSHADERPASS_API FcustomShaderPassModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RenderCustomPass(UTextureRenderTarget2D* OutputRenderTarget);

protected:
	void RenderCustomPass(FTextureRenderTargetResource* OutputRenderTarget,FRHICommandListImmediate& RHICmdList);
};
