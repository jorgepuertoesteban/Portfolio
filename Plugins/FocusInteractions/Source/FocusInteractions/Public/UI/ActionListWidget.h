// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMenu.h"
#include "FocusInteractionsTypes.h"
#include "ActionListWidget.generated.h"

class UListView;

/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UActionListWidget : public UDynamicMenu
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UListView* ActionsListView;

	UPROPERTY(Transient)
	FFocusInfo Info;

public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	bool RefreshList();

	void Configure(const FFocusInfo& NewInfo);

protected:
	UFUNCTION()
	void Close();

	virtual void WidgetLoaded(UUserWidget& NewWidget);

	UFUNCTION()
	void OnVisibilityAnimEnd_Implementation() override;
	UFUNCTION()
	void OnVisibilityAnimStart_Implementation() override;
};
