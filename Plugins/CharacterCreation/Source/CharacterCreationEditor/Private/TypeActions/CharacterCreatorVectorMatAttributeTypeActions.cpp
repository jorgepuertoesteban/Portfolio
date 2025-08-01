// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorVectorMatAttributeTypeActions.h"
#include "CharacterCreatorVectorMatAttribute.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorMatAttribute_TypeActions"

FCharacterCreatorVectorMatAttributeTypeActions::FCharacterCreatorVectorMatAttributeTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorVectorMatAttributeTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorVectorMatAttributeTypeActionsName", "Vector Material Attribute");
}

FColor FCharacterCreatorVectorMatAttributeTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FCharacterCreatorVectorMatAttributeTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorVectorMatAttribute::StaticClass();
}

uint32 FCharacterCreatorVectorMatAttributeTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorVectorMatAttributeTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE