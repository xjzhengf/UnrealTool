#include "SResScanner.h"
#include "SVersionUpdater/SVersionUpdaterWidget.h"
#include "SResScannerConfigPage.h"
// engine header
#include "Styling/SlateTypes.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "EditorStyleSet.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "IDetailsView.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "HAL/PlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#define LOCTEXT_NAMESPACE "SResScanner"

void SResScanner::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.Padding(0.0f, 10.0f, 8.0f, 0.0f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(VersionUpdaterWidget,SVersionUpdaterWidget)
					.ToolName(FText::FromString(ANSI_TO_TCHAR(TOOL_NAME)))
					.DeveloperName(FText::FromString(TEXT("lipengzha")))
					.DeveloperWebsite(FText::FromString(TEXT("https://imzlp.com")))
					.UpdateWebsite(FText::FromString(TEXT("https://imzlp.com/posts/3635/")))
					.CurrentVersion(CURRENT_VERSION_ID)
				]
				+SVerticalBox::Slot()
				.Padding(0,10,0,0)
				.AutoHeight()
				[
					SNew(SGridPanel)
					.FillColumn(1, 1.0f)

					// cook section
					+ SGridPanel::Slot(0, 0)
					.Padding(8.0f, 10.0f, 0.0f, 0.0f)
					.VAlign(VAlign_Top)
					[
					SNew(STextBlock)
					.Font(FCoreStyle::GetDefaultFontStyle("Scanner", 15))
					.Text(LOCTEXT("ResScannerSectionHeader", "Res Scanner"))
					]
					+ SGridPanel::Slot(1, 0)
					.Padding(8.0f, 0.0f, 8.0f, 0.0f)
					[
					SNew(SResScannerConfigPage)
					]
				]

			]
		]
	];

}


#undef LOCTEXT_NAMESPACE