// Created by Jorge Puerto. All rights reserved.

#include "FocusableFilterAssetTypeAction.h"
// Runtime Module
#include "FocusableFilter.h"

#define LOCTEXT_NAMESPACE "FocusInteractions_TypeActions"


FFocusableFilterAssetTypeAction::FFocusableFilterAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FFocusableFilterAssetTypeAction::GetName() const
{
	return LOCTEXT("FFocusableFilterAssetTypeActionName", "Focusable Filter");
}

FColor FFocusableFilterAssetTypeAction::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FFocusableFilterAssetTypeAction::GetSupportedClass() const
{
	return UFocusableFilter::StaticClass();
}

uint32 FFocusableFilterAssetTypeAction::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FFocusableFilterAssetTypeAction::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE