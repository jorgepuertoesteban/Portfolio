// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorOutfitTypeActions.h"
#include "CharacterCreatorOutfit.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorOutfit_TypeActions"

FCharacterCreatorOutfitTypeActions::FCharacterCreatorOutfitTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorOutfitTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorOutfitTypeActionsName", "Outfit");
}

FColor FCharacterCreatorOutfitTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FCharacterCreatorOutfitTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorOutfit::StaticClass();
}

uint32 FCharacterCreatorOutfitTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorOutfitTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE