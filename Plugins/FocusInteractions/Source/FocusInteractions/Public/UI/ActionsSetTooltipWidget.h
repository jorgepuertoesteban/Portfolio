// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionsSetTooltipWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UActionsSetTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* TooltipTextBlock;

public:

	void SetTooltipText(const FText& NewTooltipText);
};
