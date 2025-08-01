#include "GenerateCharacterCreatorGroomCommand.h"

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


bool GenerateCharacterCreatorGroomCommand::Run(const FGenerateCharacterCreatorGroomOptions& Options)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* OuterForFrame = nullptr;

	const FString SanitizedFrameName = ObjectTools::SanitizeObjectName(FString("G_").Append(Options.GroomAsset->GetName()));
	const FString LongPackagePath = FPackageName::GetLongPackagePath(Options.GroomAsset->GetOutermost()->GetPathName());
	const FString NewFlipBookDefaultPath = LongPackagePath + TEXT("/") + SanitizedFrameName;

	FString DefaultSuffix;
	FString AssetName;
	FString PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(NewFlipBookDefaultPath, DefaultSuffix, /*out*/ PackageName, /*out*/ AssetName);

	// Create a unique package name and asset name for the frame
	OuterForFrame = CreatePackage(*PackageName);
	// Create the asset
	UCharacterCreatorGroom* CharacterCreatorGroom = NewObject<UCharacterCreatorGroom>(OuterForFrame, *AssetName, RF_Public | RF_Standalone);
	FAssetRegistryModule::AssetCreated(CharacterCreatorGroom);
	CharacterCreatorGroom->MarkPackageDirty();

	if (CharacterCreatorGroom == nullptr)
	{
		return false;
	}
	TArray<UObject*> CreatedObjects;
	CreatedObjects.Add(CharacterCreatorGroom);

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(CreatedObjects);
#if WITH_EDITOR
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAssets(CreatedObjects);
#endif

	TMap<TSoftObjectPtr<USkeletalMesh>, TSoftObjectPtr<UGroomBindingAsset>> BindingsMap;

	for (USkeletalMesh* Mesh : Options.TargetSkeletalMeshes)
	{
		UGroomBindingAsset* BindingAsset = nullptr;

		BindingAsset = FHairStrandsCore::CreateGroomBindingAsset(EGroomBindingMeshType::SkeletalMesh, Options.GroomAsset, Options.SourceSkeletalMesh, Mesh, 100, 0);

		if (BindingAsset == nullptr)
		{
			continue;
		}
		BindingsMap.Add(Mesh,BindingAsset);
		BindingAsset->Build();
	}

	CharacterCreatorGroom->GroomAsset = Options.GroomAsset;
	CharacterCreatorGroom->BindingsMap = BindingsMap;


	return true;
}
