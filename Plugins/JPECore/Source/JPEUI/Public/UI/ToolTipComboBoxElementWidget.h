// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipComboBoxElementWidget.generated.h"

class UTextBlock;

USTRUCT(BlueprintType)
struct FToolTipComboBoxElementData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText FriendlyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

};

UCLASS(Abstract)
class JPEUI_API UToolTipComboBoxElementWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, FToolTipComboBoxElementData> DataByItem;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* NameTextBlock;

public:

	UFUNCTION(BlueprintCallable)
	void SetItem(const FString& Item);

};
