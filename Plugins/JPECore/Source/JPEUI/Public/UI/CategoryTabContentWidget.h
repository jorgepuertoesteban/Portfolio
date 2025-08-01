// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryTabContentWidget.generated.h"

UCLASS(Abstract)
class JPEUI_API UCategoryTabContentWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* ContentPanel;

public:

	UFUNCTION(BlueprintCallable)
	void AddEntry(UWidget* EntryWidget) const;

	UFUNCTION(BlueprintCallable)
	TArray<UWidget*> GetAllEntries() const;

	UFUNCTION(BlueprintCallable)
	void ClearEntries() const;

};