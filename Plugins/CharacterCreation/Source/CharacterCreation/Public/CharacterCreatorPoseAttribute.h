// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorPoseAttribute.generated.h"

class UCharacterCreatorOutfitSlot;

UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorPoseAttribute : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FText FriendlyName;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	const UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float Power = 1.f;
	
	UPROPERTY(VisibleAnywhere)
	TMap<FName, FTransform> MinPoseOffsets;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FTransform> MaxPoseOffsets;
};
