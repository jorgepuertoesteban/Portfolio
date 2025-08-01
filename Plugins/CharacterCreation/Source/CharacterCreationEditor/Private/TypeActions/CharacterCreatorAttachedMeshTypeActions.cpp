// Created by Jorge Puerto. All Rights Reserved.

#include "TypeActions/CharacterCreatorAttachedMeshTypeActions.h"
#include "CharacterCreatorAttachedMesh.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorAttachedMesh_TypeActions"

FCharacterCreatorAttachedMeshTypeActions::FCharacterCreatorAttachedMeshTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FCharacterCreatorAttachedMeshTypeActions::GetName() const
{
	return LOCTEXT("FCharacterCreatorAttachedMeshTypeActionsName", "AttachedMesh");
}

FColor FCharacterCreatorAttachedMeshTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FCharacterCreatorAttachedMeshTypeActions::GetSupportedClass() const
{
	return UCharacterCreatorAttachedMesh::StaticClass();
}

uint32 FCharacterCreatorAttachedMeshTypeActions::GetCategories()
{
	return AssetCategory;
}

const TArray<FText>& FCharacterCreatorAttachedMeshTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Character Creator")
	};
	return SubMenus;
}

#undef LOCTEXT_NAMESPACE