// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorTextureMatAttributeTypeActions.h"
#include "CharacterCreatorTextureMatAttribute.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorMatAttribute_TypeActions"

FCharacterCreatorTextureMatAttributeTypeActions::FCharacterCreatorTextureMatAttributeTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorTextureMatAttributeTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorTextureMatAttributeTypeActionsName", "Texture Material Attribute");
}

FColor FCharacterCreatorTextureMatAttributeTypeActions::GetTypeColor() const
{
	return FColor::Orange;
}

UClass* FCharacterCreatorTextureMatAttributeTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorTextureMatAttribute::StaticClass();
}

uint32 FCharacterCreatorTextureMatAttributeTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorTextureMatAttributeTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE