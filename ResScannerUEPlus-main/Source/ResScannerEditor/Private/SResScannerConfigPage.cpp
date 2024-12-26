// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

// #include "ResScannerPrivatePCH.h"
#include "SResScannerConfigPage.h"
#include "ResScannerProxy.h"
#include "ResScannerEditor.h"
#include "DetailCustomization/ScannerSettingsDetails.h"
#include "FlibResScannerEditorHelper.h"

// engine header
#include "Misc/FileHelper.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/SecureHash.h"
#include "HAL/FileManager.h"
#include "Kismet/KismetTextLibrary.h"

#define LOCTEXT_NAMESPACE "SResScannerConfigPage"

void SResScannerConfigPage::Construct(const FArguments& InArgs)
{
	ScannerConfig = MakeShareable(new FScannerConfig);
	CreateExportFilterListView();

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(4, 4, 10, 4)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("WhichProjectToUseText", "How would you like to scan asset the content?"))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(8.0, 0.0, 0.0, 0.0)
						[
							SNew(SButton)
							.Text(LOCTEXT("ImportConfig", "Import"))
							.OnClicked(this,&SResScannerConfigPage::DoImportConfig)
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(5.0, 0.0, 0.0, 0.0)
						[
							SNew(SButton)
							.Text(LOCTEXT("ExportConfig", "Export"))
							.OnClicked(this,&SResScannerConfigPage::DoExportConfig)
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(5.0, 0.0, 0.0, 0.0)
						[
							SNew(SButton)
							.Text(LOCTEXT("ResetConfig", "Reset"))
							.OnClicked(this, &SResScannerConfigPage::DoResetConfig)
						]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SettingsView->GetWidget()->AsShared()
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoWidth()
				.Padding(0, 0, 4, 0)
				[
					SNew(SButton)
					.Text(LOCTEXT("ScanButton", "Scan"))
					.OnClicked(this, &SResScannerConfigPage::DoScan)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
			[
				SAssignNew(ContentsWidget, SResScannerContents)
				.Visibility(EVisibility::Collapsed)
			]
		];
}

FReply SResScannerConfigPage::DoScan() const
{
	DoScanWork();
	return FReply::Handled();
}

FReply SResScannerConfigPage::DoExportConfig() const
{
	((SResScannerConfigPage*)this)->ExportConfig();
	return FReply::Handled();
}

FReply SResScannerConfigPage::DoImportConfig() const
{
	((SResScannerConfigPage*)this)->ImportConfig();
	return FReply::Handled();
}

FReply SResScannerConfigPage::DoResetConfig() const
{
	((SResScannerConfigPage*)this)->ResetConfig();
	return FReply::Handled();
}

void SResScannerConfigPage::ImportConfig()
{
	TArray<FString> Files = UFlibResScannerEditorHelper::OpenFileDialog();
	if (!Files.Num()) return;

	FString LoadFile = Files[0];

	FString JsonContent;
	if (FFileHelper::LoadFileToString(JsonContent,*LoadFile))
	{
		TemplateHelper::TDeserializeJsonStringAsStruct(JsonContent,*ScannerConfig);
		SettingsView->GetDetailsView()->ForceRefresh();
	}
}

void SResScannerConfigPage::ExportConfig()const
{

	TArray<FString> Files = UFlibResScannerEditorHelper::SaveFileDialog();

	if (!Files.Num()) return;

	FString SaveToFile = Files[0].EndsWith(TEXT(".json")) ? Files[0] : Files[0].Append(TEXT(".json"));

	if (ScannerConfig)
	{
		FString SerializedJsonStr;
		TemplateHelper::TSerializeStructAsJsonString(*ScannerConfig,SerializedJsonStr);
		FFileHelper::SaveStringToFile(SerializedJsonStr, *SaveToFile,FFileHelper::EEncodingOptions::ForceUTF8);
	}
}

void SResScannerConfigPage::ResetConfig()
{
	FString DefaultSettingJson;
	TemplateHelper::TSerializeStructAsJsonString(*FScannerConfig::Get(),DefaultSettingJson);
	TemplateHelper::TDeserializeJsonStringAsStruct(DefaultSettingJson,*ScannerConfig);
	SettingsView->GetDetailsView()->ForceRefresh();
	ContentsWidget->SetContent(TEXT(""));
	ContentsWidget->SetExpanded(false);
	ContentsWidget->SetVisibility(EVisibility::Hidden);
}

void SResScannerConfigPage::DoScanWork()const
{
	if(!ScannerConfig->bStandaloneMode)
	{
		UResScannerProxy* ScannerProxy = NewObject<UResScannerProxy>();
		ScannerProxy->AddToRoot();
		ScannerProxy->SetScannerConfig(*ScannerConfig);
		ScannerProxy->Init();
		ScannerProxy->DoScan();
		const FMatchedResult& Result = ScannerProxy->GetScanResult();
		FString OutString = ScannerProxy->SerializeResult();
		ContentsWidget->SetContent(OutString);
		ContentsWidget->SetExpanded(true);
		ContentsWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		ContentsWidget->SetContent(TEXT(""));
		ContentsWidget->SetExpanded(false);
		ContentsWidget->SetVisibility(EVisibility::Hidden);
		
		FString CurrentConfig;
		TemplateHelper::TSerializeStructAsJsonString(*ScannerConfig,CurrentConfig);
		FString SaveConfigTo = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(),TEXT("ResScanner"),TEXT("ScannerConfig.json")));
		FString NoShaderCompile = ScannerConfig->bNoShaderCompile ? TEXT("-NoShaderCompile -nullrhi") : TEXT("");
		FFileHelper::SaveStringToFile(CurrentConfig,*SaveConfigTo,FFileHelper::EEncodingOptions::ForceUTF8);
		FString MissionCommand = FString::Printf(TEXT("\"%s\" -run=ResScanner -config=\"%s\" %s %s"),*UFlibResScannerEditorHelper::GetProjectFilePath(),*SaveConfigTo,*ScannerConfig->AdditionalExecCommand,*NoShaderCompile);
		UE_LOG(LogTemp,Log,TEXT("ResScanner %s Mission: %s %s"),*ScannerConfig->ConfigName,*UFlibResScannerEditorHelper::GetUECmdBinary(),*MissionCommand);
		FResScannerEditorModule::Get().RunProcMission(UFlibResScannerEditorHelper::GetUECmdBinary(),MissionCommand,TEXT("ResScanner"));
	}
}

void SResScannerConfigPage::CreateExportFilterListView()
{
	// Create a property view
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = true;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = false;
		DetailsViewArgs.bShowOptions = true;
	}

	FStructureDetailsViewArgs StructureViewArgs;
	{
		StructureViewArgs.bShowObjects = true;
		StructureViewArgs.bShowAssets = true;
		StructureViewArgs.bShowClasses = true;
		StructureViewArgs.bShowInterfaces = true;
	}

	SettingsView = EditModule.CreateStructureDetailView(DetailsViewArgs, StructureViewArgs, nullptr);
	FStructOnScope* Struct = new FStructOnScope(FScannerConfig::StaticStruct(), (uint8*)ScannerConfig.Get());
	SettingsView->GetDetailsView()->RegisterInstancedCustomPropertyLayout(FScannerConfig::StaticStruct(),FOnGetDetailCustomizationInstance::CreateStatic(&FScannerSettingsDetails::MakeInstance));
	SettingsView->SetStructureData(MakeShareable(Struct));
}


#undef LOCTEXT_NAMESPACE
