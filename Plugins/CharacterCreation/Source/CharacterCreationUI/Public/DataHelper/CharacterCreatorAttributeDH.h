// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorAttributeDH.generated.h"

class UCharacterCreatorAttribute;
class UCharacterCreatorMatAttribute;
class UCharacterCreator;

/**
 *
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorAttributeDH : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Value = 0.f;	
	
	UPROPERTY(EditAnywhere)
	const UCharacterCreatorAttribute* CharacterCreatorAttribute;

	////Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	//UPROPERTY(EditAnywhere)
	//UCharacterCreatorMatAttribute* CharacterCreatorMaterialAttribute;

	UPROPERTY(EditAnywhere)
	UCharacterCreator* CharacterCreator;
};
