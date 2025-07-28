// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionMontageTypes.generated.h"

class UAnimMontage;

DECLARE_DELEGATE(FOnActionMontageFinishedDelegate);

USTRUCT(BlueprintType)
struct FOCUSINTERACTIONS_API FActionMontage
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionMontage)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionMontage)
	FName MontageSectionName = FName("Default");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionMontage)
	float PlaybackSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ActionMontage)
	float StartingPosition = 0.f;

};
