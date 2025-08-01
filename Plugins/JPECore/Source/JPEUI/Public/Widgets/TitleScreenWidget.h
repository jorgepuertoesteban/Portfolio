// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleScreenWidget.generated.h"

class UWidgetAnimation;

/**
 *
 */
UCLASS()
class JPEUI_API UTitleScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PressAnyButtonAnimation;

	UPROPERTY(Transient)
	bool bCanButtonBePressed = false;

	UPROPERTY(Transient)
	bool bIsClosing = false;

	UPROPERTY(EditDefaultsOnly, Category = "TitleScreen")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

public:

	virtual void NativeConstruct() override;

protected:

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void OnAnyButtonPressed();
};
