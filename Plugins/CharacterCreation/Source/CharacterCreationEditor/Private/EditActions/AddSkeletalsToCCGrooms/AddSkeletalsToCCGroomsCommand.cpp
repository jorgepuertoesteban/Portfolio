#include "AddSkeletalsToCCGroomsCommand.h"

#include "Misc/FileHelper.h"

#include "HairStrandsCore.h"
#include "GroomAsset.h"
#include "GroomBindingAsset.h"
#include "AssetToolsModule.h"
#include "PackageTools.h"
#include "ObjectTools.h"
#include "CharacterCreatorGroom.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"


bool AddSkeletalsToCCGroomsCommand::Run(const FAddSkeletalsToCCGroomsOptions& Options)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	TArray<UObject*> CreatedObjects;


	for (UCharacterCreatorGroom* CharacterCreatorGroom : Options.Grooms)
	{
		for (USkeletalMesh* Mesh : Options.Meshes)
		{
			UGroomBindingAsset* BindingAsset = nullptr;

			BindingAsset = FHairStrandsCore::CreateGroomBindingAsset(EGroomBindingMeshType::SkeletalMesh, CharacterCreatorGroom->GroomAsset.LoadSynchronous(), Options.SourceMesh, Mesh, 100, 0);

			if (BindingAsset == nullptr)
			{
				continue;
			}
			BindingAsset->Build();
			CharacterCreatorGroom->BindingsMap.Add(Mesh, BindingAsset);
			CharacterCreatorGroom->MarkPackageDirty();
			CreatedObjects.Add(CharacterCreatorGroom);
		}
	}

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(CreatedObjects);

#if WITH_EDITOR
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(CreatedObjects);
#endif

	return true;
}
