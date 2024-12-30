#pragma once
// 一些资产检查与筛选需要的配置类型和定义
// 定义为BlueprintType 给蓝图那边配置

#include "UObject/SoftObjectPath.h"

USTRUCT(BlueprintType)
struct FAssetFilters
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, DisplayName ="扫描的目录列表",meta = (RelativeToGameContentDir,LongPackageName))
	TArray<FDirectoryPath> FilterDirArr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, DisplayName ="扫描的资源列表")
	TArray<FSoftObjectPath> FilterAssetArr;
};

USTRUCT(BlueprintType)
struct FScannerConfig
{
	GENERATED_USTRUCT_BODY();

public:
	PORCEINLINE static FScannerConfig* Get() {
		static FScannerConfig StaticIns;
		return &StaticIns;
	}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName = "配置名",Category = "Base")
	FString ConfigName;

	// 是否启动全局资源搜索
	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName = "启动全局资源", Category = "Golbal")
	bool bByGlobalScanFilters = false;
};