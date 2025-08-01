// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorGroomsSetTypeActions.h"
#include "CharacterCreatorGroomsSet.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorGroomsSet_TypeActions"

FCharacterCreatorGroomsSetTypeActions::FCharacterCreatorGroomsSetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorGroomsSetTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorGroomsSetTypeActionsName", "Groom Set");
}

FColor FCharacterCreatorGroomsSetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FCharacterCreatorGroomsSetTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorGroomsSet::StaticClass();
}

uint32 FCharacterCreatorGroomsSetTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorGroomsSetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE