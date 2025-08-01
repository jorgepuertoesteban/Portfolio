// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Helpers/CreateTabParams.h"
#include "TabHelper.generated.h"

class UCategoryTabWidget;
class UWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategoryTabCreated, UCategoryTabWidget*, ButtonTab, UWidget*, Content);

/**
 * 
 */
UCLASS()
class JPEUI_API UTabHelper : public UObject
{
	GENERATED_BODY()

public:

#pragma region FOnCategoryTabCreated
	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnCategoryTabCreated OnCategoryTabCreated;
#pragma endregion
	
	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	TArray<FCreateCategoryTabParams> CategoryTabParamsArray;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	TSubclassOf<UCategoryTabWidget> TabWidgetClass;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	UWidgetSwitcher* TabContentSwitcher;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	UPanelWidget* ButtonsPanelWidget;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	UTextBlock* CategorySelectedText;

public:

	UFUNCTION(BlueprintCallable)
	bool Initialize(
		TSubclassOf<UCategoryTabWidget> NewTabWidgetClass,
		UWidgetSwitcher* NewTabContentSwitcher,
		UPanelWidget* NewTabsWidget,
		UTextBlock* NewSelectedCategoryText);

	UFUNCTION(BlueprintCallable)
	void CreateTabs();
	
	UFUNCTION(BlueprintCallable)
	void Empty();

	UFUNCTION(BlueprintCallable)
	void SetActiveTab(UCategoryTabWidget* SelectedCategoryTabWidget);

	UFUNCTION(BlueprintCallable)
	void NextTab();

	UFUNCTION(BlueprintCallable)
	void PreviousTab();

	UFUNCTION(BlueprintCallable)
	void SetActiveTabByIndex(int32 NewContentIndex);
};
