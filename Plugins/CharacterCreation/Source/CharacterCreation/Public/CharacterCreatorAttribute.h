// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorAttribute.generated.h"

class UCharacterCreatorOutfitSlot;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorAttribute : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName MorphName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName FriendlyName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float MorphMin = -1.f;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float MorphMax = 1.f;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FString DatabaseColumnName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UCharacterCreatorOutfitSlot* Slot;

};
