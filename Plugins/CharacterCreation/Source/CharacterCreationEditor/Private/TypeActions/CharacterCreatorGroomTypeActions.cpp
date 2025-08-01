// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorGroomTypeActions.h"
#include "CharacterCreatorGroom.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorGroom_TypeActions"

FCharacterCreatorGroomTypeActions::FCharacterCreatorGroomTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorGroomTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorGroomTypeActions", "Groom");
}

FColor FCharacterCreatorGroomTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FCharacterCreatorGroomTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorGroom::StaticClass();
}

uint32 FCharacterCreatorGroomTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorGroomTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE