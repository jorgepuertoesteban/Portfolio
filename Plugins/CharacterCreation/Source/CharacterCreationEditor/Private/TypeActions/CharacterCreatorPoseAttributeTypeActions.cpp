// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorPoseAttributeTypeActions.h"
#include "CharacterCreatorPoseAttribute.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorPoseAttribute_TypeActions"

FCharacterCreatorPoseAttributeTypeActions::FCharacterCreatorPoseAttributeTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorPoseAttributeTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorPoseAttributeTypeActionsName", "PoseAttribute");
}

FColor FCharacterCreatorPoseAttributeTypeActions::GetTypeColor() const
{
	return FColor::Magenta;
}

UClass* FCharacterCreatorPoseAttributeTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorPoseAttribute::StaticClass();
}

uint32 FCharacterCreatorPoseAttributeTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorPoseAttributeTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE