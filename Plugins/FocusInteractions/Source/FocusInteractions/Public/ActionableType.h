// Created by Bionic Ape. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionableType.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object, BlueprintType)
class FOCUSINTERACTIONS_API UActionableType : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionType")
	uint8 ActionableTypeValue = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionType")
	FName ActionName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionType")
	FName ActionFriendlyName = NAME_None;

public:

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FName& GetActionName() const { return ActionName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FName& GetActionFriendlyName() const { return ActionFriendlyName; }
};
