// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorColorsSet.generated.h"

class UCharacterCreatorGroom;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorColorsSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	FString ColorsSetName;

	UPROPERTY(EditInstanceOnly)
	FName FriendlyName;

	UPROPERTY(EditInstanceOnly)
	TArray<FLinearColor> Colors;

};
