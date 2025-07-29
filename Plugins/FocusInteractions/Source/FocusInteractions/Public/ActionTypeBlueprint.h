// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"
#include "ActionTypeBlueprint.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object, BlueprintType, Blueprintable, meta = (ShowWorldContextPin))
class FOCUSINTERACTIONS_API UActionTypeBlueprint : public UActionType
{
	GENERATED_BODY()
	
public:

	bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const override { return CanExecuteActionBP(ActionActor, ActionableActor); }
	bool PerformActionType(const FFocusPerformAction& Params) const override { return PerformActionTypeBP(Params); }

	UFUNCTION(BlueprintImplementableEvent, Category = "ActionType")
	bool CanExecuteActionBP(AActor* ActionActor, AActor* ActionableActor) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "ActionType")
	bool PerformActionTypeBP(FFocusPerformAction Params) const;
};
