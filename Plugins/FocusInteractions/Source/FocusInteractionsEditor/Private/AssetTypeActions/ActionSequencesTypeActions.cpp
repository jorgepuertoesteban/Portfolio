// Created by Jorge Puerto. All Rights Reserved.

#include "ActionSequencesTypeActions.h"
#include "ActionSequences.h"

#define LOCTEXT_NAMESPACE "ActionSequences_TypeActions"

FActionSequencesTypeActions::FActionSequencesTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FActionSequencesTypeActions::GetName() const
{
	return LOCTEXT("FActionSequencesTypeActionsName", "Action Sequences");
}

FColor FActionSequencesTypeActions::GetTypeColor() const
{
	return FColor::Orange;
}

UClass* FActionSequencesTypeActions::GetSupportedClass() const
{
	return UActionSequences::StaticClass();
}

uint32 FActionSequencesTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FActionSequencesTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Focus Interactions")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE