// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FocusInteractionsTypes.h"
#include "ActionListInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FOCUSINTERACTIONS_API UActionListInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FFocusInfo Info;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Index;

public:
	static UActionListInfo* NEW(FFocusInfo NewInfo, int NewIndex);
};
