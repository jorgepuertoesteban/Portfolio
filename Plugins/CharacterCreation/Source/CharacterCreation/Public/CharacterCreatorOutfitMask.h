// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreatorOutfitMask.generated.h"

class UCharacterCreatorOutfitSlot;
class UTexture;

USTRUCT(BlueprintType)
struct FCharacterCreatorOutfitMask
{
	GENERATED_BODY();

public:	

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, AdvancedDisplay)
	UTexture* TextureMask = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaterialIndex = 0;

	/**
	 * Apply this mask to the Root Skeletal Mesh
	 */
	bool bIsRoot = true;
	
	/**
	 * Only when it is not the Root Mesh (bIsRoot = false)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterCreatorOutfitSlot* OutfitSlot = nullptr;
};
