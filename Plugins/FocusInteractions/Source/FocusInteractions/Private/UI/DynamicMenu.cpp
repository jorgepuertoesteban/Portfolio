// Created by Bionic Ape. All Rights Reserved.


#include "UI/DynamicMenu.h"
#include "Animation/WidgetAnimation.h"

bool UDynamicMenu::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		if (VisibilityAnimation)
		{
			FWidgetAnimationDynamicEvent OnAnimStart;
			OnAnimStart.BindDynamic(this, &UDynamicMenu::OnVisibilityAnimStart);
			BindToAnimationStarted(VisibilityAnimation, OnAnimStart);
		}

		if (VisibilityAnimation)
		{
			FWidgetAnimationDynamicEvent OnAnimEnd;
			OnAnimEnd.BindDynamic(this, &UDynamicMenu::OnVisibilityAnimEnd);
			BindToAnimationFinished(VisibilityAnimation, OnAnimEnd);
		}
	}
	return bResult;
}

void UDynamicMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDynamicMenu::TogglePlayVisibilityAnim()
{
	bWantsToBeVisible = !bWantsToBeVisible;
	float const AnimationCurrentTime = GetAnimationCurrentTime(VisibilityAnimation);
	PlayAnimation(VisibilityAnimation, AnimationCurrentTime, 1, bWantsToBeVisible ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, AnimPlaybackSpeed);
	return bWantsToBeVisible;
}

void UDynamicMenu::HideVisibilityAnim()
{
	bWantsToBeVisible = false;
	float const AnimationCurrentTime = GetAnimationCurrentTime(VisibilityAnimation);
	PlayAnimation(VisibilityAnimation, AnimationCurrentTime, 1, bWantsToBeVisible ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, AnimPlaybackSpeed);
}

