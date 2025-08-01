// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CCPoseAttributeSlider2DWidget.h"
#include "CharacterCreator.h"
#include "CharacterCreatorPoseAttribute.h"
#include "Blueprint/UserWidget.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "UI/Synth2DSlider.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

bool UCCPoseAttributeSlider2DWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("UCategoryTabContentWidget::Initialize() Super::Initialize failed"));
		return false;
	}
	
	if (!Slider2DWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SliderWidget is null"));
		return false;
	}

	Slider2DWidget->OnValueChangedX.AddDynamic(this, &UCCPoseAttributeSlider2DWidget::OnFloatValueChanged);
	Slider2DWidget->OnValueChangedY.AddDynamic(this, &UCCPoseAttributeSlider2DWidget::OnFloatValueChanged);

	RefreshWidget();

	return true;
}

void UCCPoseAttributeSlider2DWidget::RefreshWidget()
{
	if (!XPoseAttribute)
	{
		return;
	}
	if (!YPoseAttribute)
	{
		return;
	}
	Slider2DWidget->SetValue(FVector2D(.5f));
	if (PoseAttributeTextBlock)
	{
		PoseAttributeTextBlock->SetText(XPoseAttribute->FriendlyName);
	}
	if (IconImage)
	{
		IconImage->SetBrushFromTexture(XPoseAttribute->Icon);
	}
}

void UCCPoseAttributeSlider2DWidget::OnFloatValueChanged(float Value)
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}
	FVector2D Vector2D = Slider2DWidget->GetValue();

	float XValue = FMath::GetMappedRangeValueClamped(FVector2D(0,1), FVector2D(-1,1), bInvertXAxis ? 1 - Vector2D.X : Vector2D.X);
	float YValue = FMath::GetMappedRangeValueClamped(FVector2D(0,1), FVector2D(-1,1), bInvertYAxis ? 1 - Vector2D.Y : Vector2D.Y);

	CCControlComp->Server_SetPoseAttributeValue(CCControlComp->MainCharacterCreator, XPoseAttribute, XValue);
	CCControlComp->Server_SetPoseAttributeValue(CCControlComp->MainCharacterCreator, YPoseAttribute, YValue);

}
