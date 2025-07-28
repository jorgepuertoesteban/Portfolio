// Created by Bionic Ape. All rights reserved.

#include "ActionableTypeAssetTypeAction.h"
// Runtime Module
#include "ActionableType.h"

#define LOCTEXT_NAMESPACE "FocusInteractions_TypeActions"


FActionableTypeAssetTypeAction::FActionableTypeAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FActionableTypeAssetTypeAction::GetName() const
{
	return LOCTEXT("FActionableTypeAssetTypeActionName", "Actionable Type");
}

FColor FActionableTypeAssetTypeAction::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FActionableTypeAssetTypeAction::GetSupportedClass() const
{
	return UActionableType::StaticClass();
}

uint32 FActionableTypeAssetTypeAction::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FActionableTypeAssetTypeAction::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE