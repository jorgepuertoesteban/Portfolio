// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorSlotDH.generated.h"

class UCharacterCreatorOutfitSlot;
class UCharacterCreatorMatAttribute;
class UCharacterCreator;

/**
 *
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorSlotDH : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	UCharacterCreatorMatAttribute* MaterialAttribute;

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorOutfitSlot* OutfitSlot;

	UPROPERTY(EditAnywhere)
	UCharacterCreator* CharacterCreator;

	//Might be better to just store a reference of the FAttributeValue structure
	UPROPERTY(EditAnywhere)
	TArray<const UCharacterCreatorOutfitSlot*> MaterialAttributeAffectedSlots;
};
