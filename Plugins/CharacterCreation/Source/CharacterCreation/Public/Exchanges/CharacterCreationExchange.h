// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "CharacterCreationExchange.generated.h"

class UCharacterCreator;

USTRUCT(BlueprintType, Blueprintable)
struct CHARACTERCREATION_API FSaveCharacterCreatorResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool Success = false;

	UPROPERTY(BlueprintReadOnly)
	FString Message;
};

USTRUCT(BlueprintType, Blueprintable)
struct CHARACTERCREATION_API FLoadCharacterCreatorResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool Success = false;

	UPROPERTY(BlueprintReadOnly)
	FString Message;

	UPROPERTY()
	UCharacterCreator* CharacterCreator = nullptr;
};

DECLARE_DELEGATE_OneParam(FSaveCharacterCreatorDelegate, FSaveCharacterCreatorResponse);

DECLARE_DELEGATE_OneParam(FLoadCharacterCreatorDelegate, FLoadCharacterCreatorResponse);