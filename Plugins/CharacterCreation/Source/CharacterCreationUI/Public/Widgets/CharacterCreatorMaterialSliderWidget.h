// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorMaterialSliderWidget.generated.h"

class UCharacterCreatorMatAttribute;
class USlider;
class UTextBlock;
class UCharacterCreatorMaterialSlider;
class UCharacterCreatorOutfitWidget;
class UPanelWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatAttributeChangedEvent);

UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorMaterialSliderWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorMatAttribute* MatAttribute;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USlider* SliderWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MaterialAttributeTextBlock;
	

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetMaterialAttribute(const UCharacterCreatorMatAttribute* NewMatAttribute);


	UFUNCTION()
	void OnFloatValueChanged(float Value);

	virtual void RefreshWidget();

	FOnMatAttributeChangedEvent OnMatAttributeChangedEvent;

};
