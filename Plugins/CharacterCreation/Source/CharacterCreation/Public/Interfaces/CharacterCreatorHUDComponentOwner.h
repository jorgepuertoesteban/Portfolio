// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterCreatorHUDComponentOwner.generated.h"

class UCharacterCreatorHUDComponent;

UINTERFACE()
class UCharacterCreatorHUDComponentOwner : public UInterface
{
	GENERATED_BODY()
};

class CHARACTERCREATION_API ICharacterCreatorHUDComponentOwner
{
	GENERATED_BODY()

public:
	virtual UCharacterCreatorHUDComponent* GetCharacterCreatorHUDComponent() const = 0;
};
