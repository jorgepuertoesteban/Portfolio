// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorOutfitModifier.h"
#include "CharacterCreatorModel.generated.h"

class UCharacterCreatorAttributesSet;
class UCharacterCreatorAttribute;
class UCharacterCreatorMatAttributesSet;
class UCharacterCreatorMatAttribute;
class UCharacterCreatorOutfitsSet;
class UCharacterCreatorGroomsSet;
class UCharacterCreatorOutfit;
class UCharacterCreatorOutfitSlot;
class UCharacterCreatorOutfitModifier;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorModel : public UObject
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TArray<UCharacterCreatorAttributesSet*> AttributesSets;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TMap<FString, UCharacterCreatorAttribute*> Attributes;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TArray<UCharacterCreatorMatAttributesSet*> MaterialAttributesSets;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TMap<FString, UCharacterCreatorMatAttribute*> MaterialAttributes;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TMap<UCharacterCreatorOutfitSlot*,UCharacterCreatorOutfitsSet*> OutfitSets;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TArray<UCharacterCreatorOutfitSlot*> Slots;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*> GroomSets;

	UPROPERTY(EditAnywhere)
	TArray<UCharacterCreatorOutfitModifier*> DefaultModifiers;

	bool ContainsSlot(const FString& SlotID);

	bool ContainsSlot(const UCharacterCreatorOutfitSlot* FindSlot);

	const UCharacterCreatorOutfitSlot* GetSlot(const FString& SlotID);

	//UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	//TMap<FString, UCharacterCreatorOutfitSlot*> SlotsMap;

	//UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	//TMap<FString, UCharacterCreatorOutfit*> OutfitsMap;

	//UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	//TMap<int32, UCharacterCreatorOutfit*> Outfits;
};