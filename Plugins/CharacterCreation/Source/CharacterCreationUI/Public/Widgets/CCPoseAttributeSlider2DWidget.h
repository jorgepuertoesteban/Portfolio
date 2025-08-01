// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCPoseAttributeSlider2DWidget.generated.h"

class UCharacterCreatorPoseAttribute;
class UTextBlock;
class UCharacterCreatorMaterialSlider;
class UCharacterCreatorOutfitWidget;
class UPanelWidget;
class USynth2DSlider;
class UImage;
		

UCLASS()
class CHARACTERCREATIONUI_API UCCPoseAttributeSlider2DWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorPoseAttribute* XPoseAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorPoseAttribute* YPoseAttribute;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USynth2DSlider* Slider2DWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PoseAttributeTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	bool bInvertXAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	bool bInvertYAxis;

public:

	virtual bool Initialize() override;


	UFUNCTION(BlueprintCallable)
	void OnFloatValueChanged(float Value);

	virtual void RefreshWidget();

};
