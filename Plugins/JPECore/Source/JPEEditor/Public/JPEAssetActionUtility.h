// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Engine/Blueprint.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialFunctionInterface.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Blueprint/UserWidget.h"
#include "Animation/MorphTarget.h"
#include "Components/SkeletalMeshComponent.h"
#include "JPEAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class JPEEDITOR_API UJPEAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	/**
	* Rename the selected assets
	* @param SearchPattern - the pattern to search for
	* @param ReplacePattern - the pattern to replace with
	* @param SearchCase - should the case be ignored?
	**/
	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);

	/**
	* Check's if a Texture is a power of two texture
	**/
	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();

	/**
	* Add correct prefix to Asset
	**/
	UFUNCTION(CallInEditor)
	void AddPrefixes();

	/**
	* Clean up Assets by moving them to a new folder
	**/
	UFUNCTION(CallInEditor)
	void CleanupFolder(FString ParentFolder = FString("/Game"));

	/**
	* Duplicates the selected Assets
	* NumberOfDuplicates - The amount of duplicates to create
	* bSave - Should the duplicates be saved immediately?
	**/
	UFUNCTION(CallInEditor)
	void DuplicateAsset(uint32 NumberOfDuplicates = 1, bool bSave = true);

	/**
	* Deletes unused Assets
	* bDeleteImmediately - Delete them?
	**/
	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets(bool bDeleteImmediately);

private:

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UUserWidget::StaticClass(), TEXT("WBP_")},
		{UMorphTarget::StaticClass(), TEXT("MT_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")}
	};

	bool IsPowerOfTwo(int32 NumberToCheck);

	/**
	* Prints a message to the screen
	* @param Message - the message to print
	* @param Color - the Color to use for the Message
	**/
	void PrintToScreen(FString Message, FColor Color);

	/**
	* Give Feedback to the user
	* @param Method - the Method that was used
	* @param Counter - how many objects were affected
	**/
	void GiveFeedback(FString Method, uint32 Counter);

	
};
