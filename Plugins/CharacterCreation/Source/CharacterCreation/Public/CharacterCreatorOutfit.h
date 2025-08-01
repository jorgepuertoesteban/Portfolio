// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorMaterialVariant.h"
#include "CharacterCreatorOutfitModifier.h"
#include "CharacterCreatorOutfitMask.h"
#include "CharacterCreatorOutfitMorphAttribute.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorOutfit.generated.h"

class USkeletalMesh;
class UCharacterCreatorOutfitSlot;
class UCharacterCreatorOutfitModifier;
class UTexture;

UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorOutfit : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName OutfitName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FText FriendlyName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TArray<TSoftObjectPtr<USkeletalMesh>> Meshes;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	int32 DatabaseId;

	UPROPERTY(EditAnywhere)
	TArray<FCharacterCreatorMaterialVariant> MaterialVariants;

	UPROPERTY(EditAnywhere)
	TArray<FCharacterCreatorOutfitMask> Masks;
	
	UPROPERTY(EditAnywhere)
	TArray<FCharacterCreatorOutfitMorphAttribute> Morphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FCCMOUnderConditions> ModifiersWithConditions;
	
};
