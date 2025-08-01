// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorOutfitsSet.generated.h"

class UCharacterCreatorOutfit;
class UTexture2D;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorOutfitsSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FString OutfitsSetName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FText FriendlyName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	bool bShowInUI = true;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TArray<const UCharacterCreatorOutfit*> Outfits;

public:

	const UCharacterCreatorOutfit* GetNextOutfit(const UCharacterCreatorOutfit* Outfit) const
	{
		int32 Index = Outfits.IndexOfByKey(Outfit);
		if (Index != INDEX_NONE)
		{
			Index = (Index + 1) % Outfits.Num();
			return Outfits[Index];
		}
		return Outfits[0];
	}

	const UCharacterCreatorOutfit* GetPrevOutfit(const UCharacterCreatorOutfit* Outfit) const
	{
		int32 Index = Outfits.IndexOfByKey(Outfit);
		if (Index != INDEX_NONE)
		{
			int32 const OutfitsNum = Outfits.Num();
			Index = (OutfitsNum + Index - 1) % OutfitsNum;
			return Outfits[Index];
		}
		return Outfits[0];
	}
};
