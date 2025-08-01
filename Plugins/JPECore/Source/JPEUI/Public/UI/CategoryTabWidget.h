// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JPEUIStatics.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Styling/SlateTypes.h"
#include "CategoryTabWidget.generated.h"

class UButton;
class UTextBlock;
class UButtonStyleAsset;
class UObject;
class UCategoryTabContentWidget;

#if ENGINE_MAJOR_VERSION == 4
DECLARE_EVENT_OneParam(FCCOnTabSelectedDelegate, FCCTabSelectedEvent, UCategoryTabWidget* /*SelectedWidget*/)
#elif ENGINE_MAJOR_VERSION == 5
DECLARE_EVENT_OneParam(FCCOnTabSelectedDelegate, FCCTabSelectedEvent, UCategoryTabWidget* SelectedWidget)
#endif

/**
 * 
 */
UCLASS()
class JPEUI_API UCategoryTabWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UButton* TabButton;

	UPROPERTY(meta = (BindWidgetOptional), VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextBlock* TabTextBlock;

	UPROPERTY(EditDefaultsOnly)
	FText TabCategoryName;

	UPROPERTY()
	UObject* ResourceObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ButtonStyleSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ButtonStyleNotSelected;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UCategoryTabContentWidget* ContentWidget;

#pragma region TabSelectedEvent
public:
	FCCTabSelectedEvent& OnChanged() { return TabSelectedEvent; }
private:
	FCCTabSelectedEvent TabSelectedEvent;
#pragma endregion

public:
	
	virtual bool Initialize() override;

	void Setup(const FText& TabCategoryName, UObject* ResourceObject);

	void Refresh();
	
	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
	void SetStyleSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void SetStyleNotSelected();
};
