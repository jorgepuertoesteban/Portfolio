// Created by Jorge Puerto. All Rights Reserved.

#include "ContentBrowser/JPEAssetContextMenu.h"

#include "JPEEditorLog.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ToolMenus.h"
#include "Dom/JsonObject.h"
#include "Utils/AssetUtils.h"
#include "Runtime/Launch/Resources/Version.h"

#define LOCTEXT_NAMESPACE "JPEContentBrowser"

void FJPEAssetContextMenu::Register()
{
	if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AssetContextMenu"))
	{
		FToolMenuSection& Section = Menu->FindOrAddSection("CommonAssetActions");
		Section.AddMenuEntry(
		"JPEExportAssets",
		LOCTEXT("JPEExportAssets", "JPE Asset Export..."),
		LOCTEXT("JPEExportAssetsTooltip", "Export the selected asset(s) like a JPE."),
#if ENGINE_MAJOR_VERSION == 4
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.Migrate"),
#elif ENGINE_MAJOR_VERSION == 5
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.Migrate"),
#endif
		FUIAction(
			FExecuteAction::CreateStatic(&FJPEAssetContextMenu::ExecuteExportAssets),
			FCanExecuteAction::CreateStatic(&FJPEAssetContextMenu::CanExecuteExportAssets)
			)
		);
	}
}

void FJPEAssetContextMenu::ExecuteExportAssets()
{
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	if (SelectedAssets.Num() <= 0)
	{
		UE_LOG(LogJPEEditor, Warning, TEXT("FJPEAssetContextMenu::ExecuteExportAssets: No selected assets to export."));
	}
	else if (!ExportJPEAssets(SelectedAssets))
	{
		UE_LOG(LogJPEEditor, Warning, TEXT("FJPEAssetContextMenu::ExecuteExportAssets: Something went wrong trying to export JPEAssets."));
	}
}

bool FJPEAssetContextMenu::ExportJPEAssets(const TArray<FAssetData>& AssetsData)
{
	// Generate a Json with all assets
	const TSharedPtr<FJsonObject> AssetsJson = MakeShareable(new FJsonObject());
	for (FAssetData AssetData : AssetsData)
	{
		UObject* Asset = AssetData.GetAsset();
		if (!Asset)
		{
			UE_LOG(LogJPEEditor, Error, TEXT("FJPEAssetContextMenu::ExportJPEAssets: One of the assets to be exported is not valid."));
			return false;
		}

		// Create Json and store all assets properties in it
		TSharedPtr<FJsonObject> AssetJsonObject = UAssetUtils::ToJson(Asset);
		AssetsJson->SetObjectField(Asset->GetName(), AssetJsonObject);
	}
	
	// Calculate resulting file path
	const FString NowString = FDateTime::Now().ToString().Replace(TEXT("-"),TEXT("_")).Replace(TEXT("."),TEXT("-"));
	const FString FilePath = FPaths::ProjectDir() + "Exported/" + NowString + ".baassets";
	
	// Write result to file
	return UAssetUtils::WriteJsonToFile(AssetsJson, FilePath);
}

bool FJPEAssetContextMenu::CanExecuteExportAssets()
{
	return true;
}

#undef LOCTEXT_NAMESPACE
