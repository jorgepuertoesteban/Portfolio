// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterCreatorControlComponentOwner.generated.h"

class UCharacterCreatorControlComponent;

UINTERFACE()
class UCharacterCreatorControlComponentOwner : public UInterface
{
	GENERATED_BODY()
};

class CHARACTERCREATION_API ICharacterCreatorControlComponentOwner
{
	GENERATED_BODY()

public:
	virtual UCharacterCreatorControlComponent* GetCharacterCreatorControlComponent() const = 0;
};
