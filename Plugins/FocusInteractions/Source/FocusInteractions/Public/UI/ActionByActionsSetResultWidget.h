// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusInteractionsTypes.h"
#include "ActionByActionsSetResultWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UActionByActionsSetResultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionTextBlock;

	UPROPERTY()
	FActionByActionsSetResult ActionByActionsSetResult;

public:

	virtual bool Initialize() override;

	virtual void SetActionByActionsSetResult(const FActionByActionsSetResult& NewActionByActionsSetResult);

	UFUNCTION(BlueprintCallable)
	virtual void Refresh();
};
