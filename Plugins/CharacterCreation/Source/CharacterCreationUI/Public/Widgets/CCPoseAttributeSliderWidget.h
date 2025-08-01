// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCPoseAttributeSliderWidget.generated.h"

class UCharacterCreatorPoseAttribute;
class UTextBlock;
class UCharacterCreatorMaterialSlider;
class UCharacterCreatorOutfitWidget;
class UPanelWidget;
class USlider;


UCLASS()
class CHARACTERCREATIONUI_API UCCPoseAttributeSliderWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorPoseAttribute* PoseAttribute;


	UPROPERTY(meta = (BindWidget))
	USlider* SliderWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PoseAttributeTextBlock;
	

public:

	virtual bool Initialize() override;



	UFUNCTION()
	void OnFloatValueChanged(float Value);

	virtual void RefreshWidget();

};
