// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UObject/UObjectGlobals.h"
#include "DisButton.generated.h"

class UButtonStyleAsset;
class USlateBrushAsset;

/**
 * This class is compatible with Styles when a Gamepad is used. (default UButton only works with mouse/touch)
 */
UCLASS(BlueprintType, Blueprintable, Meta=(ShowWorldContextPin))
class JPEUI_API UDisButton : public UButton
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JPEAppStyle")
	bool bUseJPEStyleAsset = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JPEAppStyle", meta = (EditCondition = "bUseJPEStyleAsset"))
	UButtonStyleAsset* JPEButtonStyleAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JPEAppStyle")
	USlateBrushAsset* FocusSlateBrushAsset;

public:

	UDisButton(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void RebuildWidgetBP();

};
