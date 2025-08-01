// Created by Jorge Puerto. All Rights Reserved.

#include "JPEUIEditor.h"
#include "TypeActions/JPEUIConfigTypeActions.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FJPEUIEditorModule"

void FJPEUIEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("JPEEditor")), LOCTEXT("JPEEditor", "JPE"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FJPEUIConfigTypeActions(AssetCategoryBit)));
}

void FJPEUIEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJPEUIEditorModule, JPEUIEditor)