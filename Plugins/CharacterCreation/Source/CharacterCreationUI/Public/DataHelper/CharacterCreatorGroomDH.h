// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorGroomDH.generated.h"

class UCharacterCreatorGroom;
class UCharacterCreatorGroomsSet;
class UCharacterCreator;
class UCharacterCreatorOutfitSlot;

/**
 *
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorGroomDH : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorGroomsSet* GroomsSet;

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorOutfitSlot* GroomSlot;

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorGroom* SelectedGroom;

	UPROPERTY(EditAnywhere)
	UCharacterCreator* CharacterCreator;
};
