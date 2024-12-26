// Copyright Epic Games, Inc. All Rights Reserved.

#include "customShaderPassBPLibrary.h"
#include "customShaderPass.h"

UcustomShaderPassBPLibrary::UcustomShaderPassBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UcustomShaderPassBPLibrary::customShaderPassSampleFunction(float Param)
{
	return -1;
}

