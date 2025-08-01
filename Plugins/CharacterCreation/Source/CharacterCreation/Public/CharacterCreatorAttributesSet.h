// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorAttributesSet.generated.h"

class UCharacterCreatorAttribute;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorAttributesSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	FString AttributesSetName;

	UPROPERTY(EditInstanceOnly)
 	TArray<const UCharacterCreatorAttribute*> Attributes;
};
