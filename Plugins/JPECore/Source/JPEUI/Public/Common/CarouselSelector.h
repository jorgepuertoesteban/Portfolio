// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CarouselSelector.generated.h"

class UButton;
class UCarouselSelectorButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCarouselSelectionChanged, const int32, NewOptionIndex);

UCLASS(Abstract)
class JPEUI_API UCarouselSelector : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnCarouselSelectionChanged OnSelectionChanged;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> Options;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SelectedOptionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCarouselSelectorButton> CarouselSelectorButtonClass;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentOptionText;
	UPROPERTY(meta=(BindWidget))
	UButton* ArrowLeft;
	UPROPERTY(meta=(BindWidget))
	UButton* ArrowRight;
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ButtonsPanel;

public:

	void SetOptions(const TArray<FString>& NewOptions);
	void SelectOption(const int32 NewOptionIndex);
	void SelectOption(const FString& NewOption);

protected:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	
	virtual bool Refresh();
	
	UFUNCTION()
	void HandleOnArrowLeftClicked();
	UFUNCTION()
	void HandleOnArrowRightClicked();
	UFUNCTION()
	void HandleOnOptionSelected(const int32 NewOptionIndex);
	
};
