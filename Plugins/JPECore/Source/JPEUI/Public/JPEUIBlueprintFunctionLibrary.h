// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JPEUIBlueprintFunctionLibrary.generated.h"

class UPanelWidget;
class UButton;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class JPEUI_API UJPEUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "JPE UI")
	static void SetOptionsTab(UPanelWidget* ButtonsPanel, UWidgetSwitcher* TabSwitcher, UButton* SelectedTabButton, UPARAM(ref) const FLinearColor& SelectedTabColor = FLinearColor::White, UPARAM(ref) const FLinearColor& UnselectedTabColor = FLinearColor::Gray);
	
};
