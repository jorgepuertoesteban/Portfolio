// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/TitleScreenWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Blueprint/UserWidget.h"

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUMGSequencePlayer* SequencePlayer = PlayAnimationForward(FadeInAnimation);
	SequencePlayer->OnSequenceFinishedPlaying().AddWeakLambda(
		this, [this](const UUMGSequencePlayer& UMGSequencePlayer)
		{
			bCanButtonBePressed = true;
			(void)PlayAnimation(PressAnyButtonAnimation, 0.f, 0, EUMGSequencePlayMode::PingPong);
		}
	);
}

FReply UTitleScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	OnAnyButtonPressed();
	return FReply::Handled();
}

FReply UTitleScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnAnyButtonPressed();
	return FReply::Handled();
}

void UTitleScreenWidget::OnAnyButtonPressed()
{
	if (bCanButtonBePressed && !bIsClosing && MainMenuWidgetClass)
	{
		bIsClosing = true;

		UUMGSequencePlayer* SequencePlayer = PlayAnimationForward(FadeOutAnimation);
		SequencePlayer->OnSequenceFinishedPlaying().AddWeakLambda(
			this, [this](const UUMGSequencePlayer& UMGSequencePlayer)
			{
				UUserWidget* BackendAlertWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), MainMenuWidgetClass);
				BackendAlertWidget->AddToViewport();
				RemoveFromParent();
			}
		);
	}
}
