#pragma once

#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "Shader.h"

class FCustomPSShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FCustomPSShader,Global);

public:
	FCustomPSShader();

	FCustomPSShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer):FGlobalShader(Initializer){

	};

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& Environment);
};



class FCustomVSShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FCustomVSShader, Global);

public:
	FCustomVSShader();

	FCustomVSShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) {

	};

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& Environment);
};




FCustomPSShader::FCustomPSShader()
{
}


bool FCustomPSShader::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

void FCustomPSShader::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& Environment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, Environment);
}

FCustomVSShader::FCustomVSShader()
{
}



bool FCustomVSShader::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

void FCustomVSShader::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& Environment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, Environment);
}

