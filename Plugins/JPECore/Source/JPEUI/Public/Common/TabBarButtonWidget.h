// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabBarButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabBarButtonClicked, const FText&, TabName);

UCLASS(Abstract)
class JPEUI_API UTabBarButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnTabBarButtonClicked OnTabBarButtonClicked;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TabName;

	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Label;

public:

	void SetTabName(const FText& NewTabName);
	void Hover() const;
	void Unhover() const;

protected:

	virtual void NativeConstruct() override;

	virtual bool Refresh();

	UFUNCTION()
	void HandleButtonClicked();
	
};
