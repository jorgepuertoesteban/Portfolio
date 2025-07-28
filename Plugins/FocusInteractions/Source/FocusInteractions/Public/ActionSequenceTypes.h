// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelSequence.h"
#include "ActionSequenceTypes.generated.h"

DECLARE_DELEGATE(FOnActionSequenceFinishedDelegate);
DECLARE_DELEGATE(FOnActionSequenceEventDelegate);

USTRUCT(BlueprintType)
struct FOCUSINTERACTIONS_API FActionSequenceBindingTag
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionSequence)
	FName Tag = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionSequence)
	UObject* Object = nullptr;
};

USTRUCT(BlueprintType)
struct FOCUSINTERACTIONS_API FActionSequenceBindingParams
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionSequence)
	ULevelSequence* Sequence = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionSequence)
	TArray<FActionSequenceBindingTag> ActionSequenceBindingTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionSequence)
	FTransform SequenceRoot;

};