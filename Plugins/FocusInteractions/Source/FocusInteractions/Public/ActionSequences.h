// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionSequences.generated.h"

class UActionType;
class ULevelSequence;

USTRUCT(BlueprintType)
struct FActionLevelSequences
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSoftObjectPtr<ULevelSequence>> Sequences;
};

UCLASS()
class FOCUSINTERACTIONS_API UActionSequences : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sequences)
	TMap<UActionType*, FActionLevelSequences> ActionSequences;
};
