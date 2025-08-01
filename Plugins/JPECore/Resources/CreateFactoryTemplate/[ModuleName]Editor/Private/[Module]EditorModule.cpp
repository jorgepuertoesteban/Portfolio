// Created by Jorge Puerto. All Rights Reserved.

#include "[Module]EditorModule.h"
#include "IAssetTools.h"
#include "TypeActions/[Factory]TypeActions.h"

#define LOCTEXT_NAMESPACE "F[Module]EditorModuleModule"

void F[Module]EditorModuleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	const EAssetTypeCategories::Type AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("JPEEditor")), LOCTEXT("JPEEditor", "Bionic Ape"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new F[Factory]TypeActions(AssetCategoryBit)));
}

void F[Module]EditorModuleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(F[Module]EditorModuleModule, [Module]EditorModule)