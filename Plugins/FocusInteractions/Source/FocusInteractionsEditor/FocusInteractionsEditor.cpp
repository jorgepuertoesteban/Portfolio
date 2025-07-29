// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FocusInteractionsEditor.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions/ActionTypeAssetTypeAction.h"
#include "Templates/SharedPointer.h"
#include "AssetTypeActions/ActionableTypeAssetTypeAction.h"
#include "AssetTypeActions/ActionSequencesTypeActions.h"
#include "AssetTypeActions/ActionMontagesTypeActions.h"
#include "AssetTypeActions/ActionsSetAssetTypeAction.h"
#include "AssetTypeActions/FocusableFilterAssetTypeAction.h"
#include "Thumbnails/ActionsSetThumbnail.h"
#include "Thumbnails/ActionTypeThumbnail.h"
#include "ActionsSet.h"
#include "ActionType.h"

#define LOCTEXT_NAMESPACE "FFocusInteractionsEditorModule"

void FFocusInteractionsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("BionicApeEditor")), LOCTEXT("BionicApeEditor", "Jorge Puerto"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FActionTypeAssetTypeAction(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FActionableTypeAssetTypeAction(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FActionSequencesTypeActions(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FActionMontagesTypeActions(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FActionsSetAssetTypeAction(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FFocusableFilterAssetTypeAction(AssetCategoryBit)));

	UThumbnailManager::Get().RegisterCustomRenderer(UActionsSet::StaticClass(), UActionsSetThumbnail::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UActionType::StaticClass(), UActionTypeThumbnail::StaticClass());

}

void FFocusInteractionsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFocusInteractionsEditorModule, FocusInteractionsEditor)