// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CarouselSelectorButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCarouselOptionSelected, const int32, NewOptionIndex);

class UBorder;
class UButton;

UCLASS(Abstract)
class JPEUI_API UCarouselSelectorButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnCarouselOptionSelected OnOptionSelected;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 OptionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor HoverColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor UnhoverColor;

	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	UPROPERTY(meta=(BindWidget))
	UBorder* Border;
	
public:

	void SetOptionIndex(const int32 NewOptionIndex);
	
	void Hover() const;
	void Unhover() const;

protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandleOnButtonClicked();
	
};
