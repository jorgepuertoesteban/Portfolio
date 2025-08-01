// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorOutfitsSetTypeActions.h"
#include "CharacterCreatorOutfitsSet.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorOutfitsSet_TypeActions"

FCharacterCreatorOutfitsSetTypeActions::FCharacterCreatorOutfitsSetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorOutfitsSetTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorOutfitsSetTypeActionsName", "Outfit Set");
}

FColor FCharacterCreatorOutfitsSetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FCharacterCreatorOutfitsSetTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorOutfitsSet::StaticClass();
}

uint32 FCharacterCreatorOutfitsSetTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorOutfitsSetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE