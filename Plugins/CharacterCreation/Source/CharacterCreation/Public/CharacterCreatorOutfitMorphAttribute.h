// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreatorOutfitMorphAttribute.generated.h"

class UCharacterCreatorOutfitSlot;

USTRUCT(BlueprintType)
struct FCharacterCreatorOutfitMorphAttribute
{
	GENERATED_BODY();

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MorphName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterCreatorOutfitSlot* OutfitSlot = nullptr;
};
