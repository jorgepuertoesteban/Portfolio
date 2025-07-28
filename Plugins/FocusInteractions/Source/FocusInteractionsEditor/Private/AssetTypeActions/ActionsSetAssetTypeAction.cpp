// Created by Bionic Ape. All rights reserved.

#include "ActionsSetAssetTypeAction.h"
// Runtime Module
#include "ActionsSet.h"

#define LOCTEXT_NAMESPACE "FocusInteractions_TypeActions"


FActionsSetAssetTypeAction::FActionsSetAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FActionsSetAssetTypeAction::GetName() const
{
	return LOCTEXT("FActionsSetAssetTypeActionName", "Actions Set");
}

FColor FActionsSetAssetTypeAction::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FActionsSetAssetTypeAction::GetSupportedClass() const
{
	return UActionsSet::StaticClass();
}

uint32 FActionsSetAssetTypeAction::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FActionsSetAssetTypeAction::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE