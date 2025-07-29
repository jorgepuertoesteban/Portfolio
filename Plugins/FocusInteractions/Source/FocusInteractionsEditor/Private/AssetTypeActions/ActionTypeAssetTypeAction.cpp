// Created by Jorge Puerto. All rights reserved.

#include "ActionTypeAssetTypeAction.h"
// Runtime Module
#include "ActionType.h"

#define LOCTEXT_NAMESPACE "FocusInteractions_TypeActions"


FActionTypeAssetTypeAction::FActionTypeAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FActionTypeAssetTypeAction::GetName() const
{
	return LOCTEXT("FActionTypeAssetTypeActionName", "Action Type");
}

FColor FActionTypeAssetTypeAction::GetTypeColor() const
{
	return FColor::Purple;
}

UClass* FActionTypeAssetTypeAction::GetSupportedClass() const
{
	return UActionType::StaticClass();
}

uint32 FActionTypeAssetTypeAction::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FActionTypeAssetTypeAction::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE