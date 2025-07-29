// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"
#include "ActionType_SequenceBase.generated.h"

UCLASS()
class FOCUSINTERACTIONS_API UActionType_SequenceBase : public UActionType
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	bool bRestoreStateAtSequenceFinish = true;

};
