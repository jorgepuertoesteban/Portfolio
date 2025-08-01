// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsItemWidget.generated.h"

class UBorder;
class UPanelWidget;
class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class JPEUI_API USettingsItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	FLinearColor OddRowColor = FLinearColor(0.f,0.f,0.f,0.3);
	UPROPERTY(EditAnywhere)
	FLinearColor EvenRowColor = FLinearColor(0.f,0.f,0.f,0.4);

	UPROPERTY(meta=(BindWidget))
	UBorder* Background;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameLabel;
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ModifierPanel;

protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	virtual bool Refresh();
	virtual void UpdateUIValues() {}
	
	UFUNCTION()
	void HandleOnJPEGameUserSettingsLoadedOrApplied();
	
};