#pragma once
// һЩ�ʲ������ɸѡ��Ҫ���������ͺͶ���
// ����ΪBlueprintType ����ͼ�Ǳ�����

#include "UObject/SoftObjectPath.h"

USTRUCT(BlueprintType)
struct FAssetFilters
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, DisplayName ="ɨ���Ŀ¼�б�",meta = (RelativeToGameContentDir,LongPackageName))
	TArray<FDirectoryPath> FilterDirArr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, DisplayName ="ɨ�����Դ�б�")
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName = "������",Category = "Base")
	FString ConfigName;

	// �Ƿ�����ȫ����Դ����
	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName = "����ȫ����Դ", Category = "Golbal")
	bool bByGlobalScanFilters = false;
};