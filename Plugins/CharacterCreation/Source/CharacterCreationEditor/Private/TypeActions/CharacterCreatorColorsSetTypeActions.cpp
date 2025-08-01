// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorColorsSetTypeActions.h"
#include "CharacterCreatorColorsSet.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorColorsSet_TypeActions"

FCharacterCreatorColorsSetTypeActions::FCharacterCreatorColorsSetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorColorsSetTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorColorsSetTypeActionsName", "Color Set");
}

FColor FCharacterCreatorColorsSetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FCharacterCreatorColorsSetTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorColorsSet::StaticClass();
}

uint32 FCharacterCreatorColorsSetTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorColorsSetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE