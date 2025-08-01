// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorMatAttributesSetTypeActions.h"
#include "CharacterCreatorMatAttributeSet.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorMatAttributesSet_TypeActions"

FCharacterCreatorMatAttributesSetTypeActions::FCharacterCreatorMatAttributesSetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorMatAttributesSetTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorMatAttributesSetTypeActionsName", "Material Attribute Set");
}

FColor FCharacterCreatorMatAttributesSetTypeActions::GetTypeColor() const
{
	return FColor::Emerald;
}

UClass* FCharacterCreatorMatAttributesSetTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorMatAttributesSet::StaticClass();
}

uint32 FCharacterCreatorMatAttributesSetTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorMatAttributesSetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE