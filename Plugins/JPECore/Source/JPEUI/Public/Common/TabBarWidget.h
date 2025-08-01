// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabBarWidget.generated.h"

class UButton;
class UTabBarButtonWidget;
class UTextBlock;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabSelected, const int32, NewTabIndex);

UCLASS(Abstract)
class JPEUI_API UTabBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnTabSelected OnTabSelected;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> TabNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UTabBarButtonWidget> TabButtonWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* TabButtonsPanel;
	UPROPERTY()
	TArray<UTabBarButtonWidget*> TabButtons;
	UPROPERTY(Transient)
	int32 ActiveTabIndex = -1;

public:

	void SetTabNames(const TArray<FText>& NewTabNames);
	
	void SelectTab(const int32 NewTabIndex);

protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	virtual bool Refresh();

	UFUNCTION()
	void HandleOnTabBarButtonClicked(const FText& TabName);
	
};
