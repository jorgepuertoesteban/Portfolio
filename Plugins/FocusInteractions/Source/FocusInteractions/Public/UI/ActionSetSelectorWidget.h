// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionSetSelectorWidget.generated.h"

class UButtonStyleAsset;
class UActionsSetTooltipWidget;
class UImage;
class UActionsSet;
class UButton;
/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UActionSetSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BAAppStyle", meta = (EditCondition = "bUseBAStyleAsset"))
	UButtonStyleAsset* NormalButtonStyleAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BAAppStyle", meta = (EditCondition = "bUseBAStyleAsset"))
	UButtonStyleAsset* SelectedButtonStyleAsset;

	UPROPERTY(Transient)
	FButtonStyle NormalStyle;

	UPROPERTY(Transient)
	FButtonStyle SelectedStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FocusInteractions")
	bool bIsSelected = false;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* Button;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	UMaterialInterface* NormalMaster;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* NormalMID;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	UMaterialInterface* HoveredMaster;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* HoveredMID;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	UMaterialInterface* DisabledMaster;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DisabledMID;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	UMaterialInterface* PressedMaster;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* PressedMID;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	UActionsSet* ActionsSet;

	UPROPERTY(EditDefaultsOnly, Category="FocusInteractions")
	TSubclassOf<UActionsSetTooltipWidget> ActionsSetTooltipWidgetClass;

public:
	virtual bool Initialize() override;
};
