// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToggleButton.generated.h"

class UBorder;
class UButton;
class USlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleButtonValueChanged, const bool, NewValue);

UCLASS(Abstract)
class JPEUI_API UToggleButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnToggleButtonValueChanged OnValueChanged;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Value = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor OffBackgroundColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor OnBackgroundColor;

	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	UPROPERTY(meta=(BindWidget))
	UBorder* Background;
	UPROPERTY(meta=(BindWidget))
	USlider* Toggler;

public:

	void SetValue(const bool NewValue);
	
protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	virtual bool Refresh(const float RefreshValue);

	UFUNCTION()
	void HandleOnButtonClicked();
	
};
