// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorMatAttributeSet.generated.h"

class UCharacterCreatorMatAttribute;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorMatAttributesSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	FString AttributesSetName;

	UPROPERTY(EditInstanceOnly)
	TArray<UCharacterCreatorMatAttribute*> Attributes;
};
