// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorGroomsSet.generated.h"

class UCharacterCreatorGroom;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorGroomsSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	FString GroomsSetName;

	UPROPERTY(EditInstanceOnly)
	FName FriendlyName;

	UPROPERTY(EditInstanceOnly)
	TArray<UCharacterCreatorGroom*> Grooms;

public:

	UCharacterCreatorGroom* GetNextGroom(const UCharacterCreatorGroom* Groom) const
	{
		int32 Index = Grooms.IndexOfByKey(Groom);
		if (Index != INDEX_NONE)
		{
			Index = (Index + 1) % Grooms.Num();
			return Grooms[Index];
		}
		return Grooms[0];
	}

	UCharacterCreatorGroom* GetPrevGroom(const UCharacterCreatorGroom* Groom) const
	{
		int32 Index = Grooms.IndexOfByKey(Groom);
		if (Index != INDEX_NONE)
		{
			int32 const GroomsNum = Grooms.Num();
			Index = (GroomsNum + Index - 1) % GroomsNum;
			return Grooms[Index];
		}
		return Grooms[0];
	}
};
