// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionsSet.generated.h"

class UActionableType;
class UActionType;
class UTexture2D;

/**
 * 
 */
UCLASS(hidecategories = Object, BlueprintType)
class FOCUSINTERACTIONS_API UActionsSet : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	FName ActionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	FText ActionFriendlyName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	FName InputKeyName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	FText InputKeyFriendlyName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	TMap<UActionableType*, UActionType*> Actions;

public:

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	UTexture2D* GetIcon() const { return Icon; }

	virtual UActionType* GetActionType(UActionableType* ActionableType, float Distance) const;

	virtual UActionType* GetActionType(UActionableType* ActionableType) const;

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FName& GetActionName() const { return ActionName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FText& GetActionFriendlyName() const { return ActionFriendlyName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FName& GetInputKeyName() const { return InputKeyName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FText& GetInputKeyFriendlyName() const { return InputKeyFriendlyName; }

	TArray<UActionType*> GetAllActions() const;
};
