// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionMontageTypes.h"
#include "ActionMontages.generated.h"

class UActionType;
class UAnimMontage;


UCLASS()
class FOCUSINTERACTIONS_API UActionMontages : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<UActionType*, FActionMontage> Montages;
};
