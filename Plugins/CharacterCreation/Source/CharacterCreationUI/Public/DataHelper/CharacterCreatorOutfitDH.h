// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorOutfitDH.generated.h"

class UCharacterCreatorOutfit;
class UCharacterCreatorOutfitsSet;
class UCharacterCreatorOutfitSlot;
class UCharacterCreator;

/**
 *
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorOutfitDH : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorOutfitsSet* OutfitsSet;

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorOutfitSlot* OutfitSlot;

	UPROPERTY(EditAnywhere)
	const UCharacterCreatorOutfit* SelectedOutfit;

	UPROPERTY(EditAnywhere)
	UCharacterCreator* CharacterCreator;
};
