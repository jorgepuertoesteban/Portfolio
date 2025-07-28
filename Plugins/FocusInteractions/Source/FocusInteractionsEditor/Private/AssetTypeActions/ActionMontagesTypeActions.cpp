// Created by Bionic Ape. All Rights Reserved.

#include "ActionMontagesTypeActions.h"
#include "ActionMontages.h"

#define LOCTEXT_NAMESPACE "ActionMontages_TypeActions"

FActionMontagesTypeActions::FActionMontagesTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FActionMontagesTypeActions::GetName() const
{
	return LOCTEXT("FActionMontagesTypeActionsName", "Action Montages");
}

FColor FActionMontagesTypeActions::GetTypeColor() const
{
	return FColor::Purple;
}

UClass* FActionMontagesTypeActions::GetSupportedClass() const
{
	return UActionMontages::StaticClass();
}

uint32 FActionMontagesTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FActionMontagesTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE