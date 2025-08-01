// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/JPEUIConfigTypeActions.h"
#include "JPEUIConfig.h"

#define LOCTEXT_NAMESPACE "JPEUIConfig_TypeActions"

FJPEUIConfigTypeActions::FJPEUIConfigTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FJPEUIConfigTypeActions::GetName() const
{
	return LOCTEXT("FJPEUIConfigTypeActionsName", "JPEUIConfig");
}

FColor FJPEUIConfigTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FJPEUIConfigTypeActions::GetSupportedClass() const
{
	return UJPEUIConfig::StaticClass();
}

uint32 FJPEUIConfigTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FJPEUIConfigTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("User Interface")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE