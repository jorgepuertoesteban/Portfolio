// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorAttributesSetTypeActions.h"
#include "CharacterCreatorAttributesSet.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorAttributesSet_TypeActions"

FCharacterCreatorAttributesSetTypeActions::FCharacterCreatorAttributesSetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorAttributesSetTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorAttributesSetTypeActionsName", "Attribute Set");
}

FColor FCharacterCreatorAttributesSetTypeActions::GetTypeColor() const
{
	return FColor::Emerald;
}

UClass* FCharacterCreatorAttributesSetTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorAttributesSet::StaticClass();
}

uint32 FCharacterCreatorAttributesSetTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorAttributesSetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE