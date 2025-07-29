// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FocusInteractionsTypes.h"
#include "ActionType.generated.h"




UCLASS(hidecategories = Object, BlueprintType)
class FOCUSINTERACTIONS_API UActionType : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ActionType")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	uint8 ActionTypeValue;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	FText ActionFriendlyName;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	float MaxDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bIsExecutedOnServer = false;


public:
	
	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetIcon(UTexture2D* NewIcon){ Icon = NewIcon;}

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetActionTypeValue(uint8 NewActionTypeValue){ ActionTypeValue = NewActionTypeValue;}

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetActionName(FName NewActionName){ ActionName = NewActionName;}

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetActionFriendlyName(FText NewActionFriendlyName){ ActionFriendlyName = NewActionFriendlyName;}

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetMaxDistance(float NewMaxDistance){ MaxDistance = NewMaxDistance;}

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetIsExecutedOnServer(bool bNewIsExecutedOnServer){ bIsExecutedOnServer = bNewIsExecutedOnServer;}
	
	UFUNCTION(BlueprintCallable, Category = "ActionType")
	UTexture2D* GetIcon() const { return Icon; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	bool IsExecutedOnServer() const { return bIsExecutedOnServer; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	uint8 GetActionTypeValue() const { return ActionTypeValue; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	const FName& GetActionName() const { return ActionName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual const FText GetActionFriendlyName(AActor* ActionableActor = nullptr) const { return ActionFriendlyName; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	float GetMaxDistance() const { return MaxDistance; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const { return true; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual bool PerformActionType(const FFocusPerformAction& Params) const { return true; }

	virtual void StartFocus(const UFocusComponent* FocusComponent, const FActionByActionsSetResult& Result) const { }

	virtual void EndFocus(const UFocusComponent* FocusComponent, const FActionByActionsSetResult& Result) const { }
};
