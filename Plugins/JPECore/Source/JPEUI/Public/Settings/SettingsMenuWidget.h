// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuWidget.generated.h"

class UButton;
class UPanelWidget;
class URevertAppliedSettingsAlertWidget;
class USettingsTabBodyWidget;
class UTabBarWidget;
class UTextBlock;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS(Abstract)
class JPEUI_API USettingsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URevertAppliedSettingsAlertWidget> RevertAppliedSettingsAlertWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UTabBarWidget* TabBar;
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* TabsWidgetSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* OptionButtonsPanel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowApplyButton = true;
	UPROPERTY(meta=(BindWidget))
	UButton* ApplyButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ApplyTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowCloseButton = true;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CloseTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowRevertButton = true;
	UPROPERTY(meta=(BindWidget))
	UButton* RevertButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RevertTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShowResetToDefaultsButton = true;
	UPROPERTY(meta=(BindWidget))
	UButton* ResetToDefaultsButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ResetToDefaultsTextBlock;

public:
	
	UFUNCTION(BlueprintCallable)
	void SelectTab(const int32 NewTabIndex) const;

	UFUNCTION(BlueprintCallable)
	void ApplyChanges();
	UFUNCTION(BlueprintCallable)
	void RevertChanges();
	UFUNCTION(BlueprintCallable)
	void ResetToDefaults();
	
protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	virtual bool Refresh(const int32 NewTabIndex);
	
	UFUNCTION()
	void HandleOnTabSelected(const int32 NewTabIndex);
	
	UFUNCTION()
	void HandleOnApplyButtonClicked();
	UFUNCTION()
	void HandleOnCloseButtonClicked();
	UFUNCTION()
	void HandleOnRevertButtonClicked();
	UFUNCTION()
	void HandleOnResetToDefaultsButtonClicked();
	UFUNCTION()
	void HandleOnSettingsApplied();
	
};
