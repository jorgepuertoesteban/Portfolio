// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorGroom.generated.h"

class UGroomAsset;
class UCharacterCreatorOutfitSlot;
class UGroomBindingAsset;
class USkeletalMesh;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorGroom : public UObject
{
	GENERATED_BODY()

public:


	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(EditInstanceOnly)
	FName OutfitName;

	UPROPERTY(EditInstanceOnly)
	FText FriendlyName;

	UPROPERTY(EditInstanceOnly)
	const UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(EditInstanceOnly)
	const UCharacterCreatorOutfitSlot* SlotToAttach;

	UPROPERTY(EditInstanceOnly)
	TMap<TSoftObjectPtr<USkeletalMesh>, TSoftObjectPtr<UGroomBindingAsset>> BindingsMap;

	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<UGroomAsset> GroomAsset;

	UPROPERTY(EditInstanceOnly)
	TArray<FCCTextureMaterialAttributeValue> TextureMaterialAttributeValues;

	UPROPERTY(EditInstanceOnly)
	int32 DatabaseId;
};
