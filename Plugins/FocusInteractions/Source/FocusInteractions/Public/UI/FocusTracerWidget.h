// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusInteractionsTypes.h"
#include "FocusTracerWidget.generated.h"

class UFocusableComponent;
class UPanelWidget;
class UActionByActionsSetResultWidget;

class UFocusComponent;
/**
 *
 */
UCLASS()
class FOCUSINTERACTIONS_API UFocusTracerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FocusTracer)
	bool bUsePawn = true;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ActionsPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActionByActionsSetResultWidget> DefaultActionByActionsSetResultWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UActionByActionsSetResultWidget>> ActionByActionsSetResultWidgetClasses;

public:

	virtual bool Initialize() override;

	UFUNCTION()
	virtual void NewFocus(const FFocusInfo& Info);

	UFUNCTION()
	virtual void EndFocus(const UFocusableComponent* Focusable);

	UFUNCTION(BlueprintImplementableEvent, Category = FocusTracer)
	void OnNewTracerFocus(const FFocusInfo& Info);

	UFUNCTION(BlueprintImplementableEvent, Category = FocusTracer)
	void OnEndTracerFocus(const UFocusableComponent* Focusable);

	virtual void Configure();
	virtual void ClearPanel();
};
