// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorModelTypeActions.h"
#include "CharacterCreatorModel.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorModel_TypeActions"

FCharacterCreatorModelTypeActions::FCharacterCreatorModelTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorModelTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorModelTypeActionsName", "Model");
}

FColor FCharacterCreatorModelTypeActions::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FCharacterCreatorModelTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorModel::StaticClass();
}

uint32 FCharacterCreatorModelTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorModelTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE