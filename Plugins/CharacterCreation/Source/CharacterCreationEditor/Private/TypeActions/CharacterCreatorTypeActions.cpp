// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorTypeActions.h"
#include "EditorToolkits/CharacterCreatorToolkit.h"
#include "CharacterCreator.h"

#define LOCTEXT_NAMESPACE "CharacterCreator_TypeActions"

FCharacterCreatorTypeActions::FCharacterCreatorTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorTypeActionsName", "Character Creator");
}

FColor FCharacterCreatorTypeActions::GetTypeColor() const
{
	return FColor::Blue;
}

UClass* FCharacterCreatorTypeActions::GetSupportedClass() const
{
	return UCharacterCreator::StaticClass();
}

uint32 FCharacterCreatorTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

void FCharacterCreatorTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FCharacterCreatorToolkit::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
}

#undef LOCTEXT_NAMESPACE