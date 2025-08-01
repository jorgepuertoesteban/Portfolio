// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreatorMaterialAttributeDH.generated.h"

class UCharacterCreator;
class UCharacterCreatorMatAttribute;

/**
 *
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorMaterialAttributeDH : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Value = 0.f;	

	UPROPERTY(EditAnywhere)
	UCharacterCreatorMatAttribute* CharacterCreatorMaterialAttribute;

	UPROPERTY(EditAnywhere)
	UCharacterCreator* CharacterCreator;
};
