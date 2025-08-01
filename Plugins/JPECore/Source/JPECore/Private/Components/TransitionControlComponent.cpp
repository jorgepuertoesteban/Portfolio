// Created by Jorge Puerto. All Rights Reserved.


#include "Components/TransitionControlComponent.h"
#include "Engine/JPEGameViewportClient.h"

UTransitionControlComponent::UTransitionControlComponent()
{
	SetIsReplicatedByDefault(true);
}

void UTransitionControlComponent::BeginPlay()
{
	Super::BeginPlay();
	const APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		//Only for Local Players
		return;
	}

	if (FadeOutOnBeginPlay)
	{
		if (FMath::IsNearlyZero(BeginPlayInitialDelay))
		{
			if (UJPEGameViewportClient* Viewport = Cast<UJPEGameViewportClient>(GetWorld()->GetGameViewport()))
			{
				Viewport->SetJPEDisableWorldRendering(false);
			}
			LocalFadeIn();
		}
		else
		{
			FTimerHandle UnusedHandle;
			GetOwner()->GetWorldTimerManager().SetTimer(
				UnusedHandle, FTimerDelegate::CreateWeakLambda(
					this, [this]()
					{
						if (UJPEGameViewportClient* Viewport = Cast<UJPEGameViewportClient>(GetWorld()->GetGameViewport()))
						{
							Viewport->SetJPEDisableWorldRendering(false);
						}
						LocalFadeIn();
					}
				), BeginPlayInitialDelay, false
			);
		}
	}
}

void UTransitionControlComponent::BeginDestroy()
{
	Super::BeginDestroy();
	if (UJPEGameViewportClient* Viewport = Cast<UJPEGameViewportClient>(GetWorld()->GetGameViewport()))
	{
		Viewport->SetJPEDisableWorldRendering(true);
	}
}

APlayerController* UTransitionControlComponent::GetPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

void UTransitionControlComponent::SendClientFadeAck(const bool bIsFadeIn, const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	const int32 DelegateKey = NextDelegateKey();
	DelegatesMap.Emplace(DelegateKey, FadeCompletedDelegate);
	Client_FadeAck(bIsFadeIn, DelegateKey);
}

void UTransitionControlComponent::SendClientFadeInAck(const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	SendClientFadeAck(true, FadeCompletedDelegate);
}

void UTransitionControlComponent::SendClientFadeOutAck(const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	SendClientFadeAck(false, FadeCompletedDelegate);
}

void UTransitionControlComponent::LocalFadeAck(const bool bIsFadeIn, const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	const int32 DelegateKey = NextDelegateKey();
	DelegatesMap.Emplace(DelegateKey, FadeCompletedDelegate);
	FadeAck(bIsFadeIn, DelegateKey, true);
}

void UTransitionControlComponent::LocalFadeInAck(const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	LocalFadeAck(true, FadeCompletedDelegate);
}

void UTransitionControlComponent::LocalFadeOutAck(const FFadeCompletedDelegate& FadeCompletedDelegate)
{
	LocalFadeAck(false, FadeCompletedDelegate);
}

void UTransitionControlComponent::LocalFadeIn() const
{
	StartCameraFade(true);
}

void UTransitionControlComponent::LocalFadeOut() const
{
	StartCameraFade(false);
}

void UTransitionControlComponent::SendClientFadeIn()
{
	Client_Fade(true);
}

void UTransitionControlComponent::SendClientFadeOut()
{
	Client_Fade(false);
}

void UTransitionControlComponent::Client_Fade_Implementation(const bool bIsFadeIn)
{
	StartCameraFade(bIsFadeIn);
}

void UTransitionControlComponent::Client_FadeAck_Implementation(const bool bIsFadeIn, const int32 DelegateKey)
{
	FadeAck(bIsFadeIn, DelegateKey, false);
}

void UTransitionControlComponent::FadeAck(const bool bIsFadeIn, const int32 DelegateKey, const bool bIsLocal)
{
	StartCameraFade(bIsFadeIn);
	FTimerHandle UnusedHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(
		UnusedHandle, FTimerDelegate::CreateWeakLambda(
			this, [this, DelegateKey, bIsLocal]()
			{
				if (bIsLocal)
				{
					NotifyFadeCompleted(DelegateKey);
				}
				else
				{
					Server_NotifyFadeCompleted(DelegateKey);
				}
			}
		), FadeDuration + AckDelay + InitialDelay, false
	);
}

void UTransitionControlComponent::NotifyFadeCompleted(const int32 DelegateKey)
{
	if (const FFadeCompletedDelegate* Delegate = DelegatesMap.Find(DelegateKey))
	{
		Delegate->Execute();
		DelegatesMap.Remove(DelegateKey);
	}
}

void UTransitionControlComponent::CreateFade(const bool bIsFadeIn) const
{
	const APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return;
	}
	PlayerController->PlayerCameraManager->StartCameraFade(
		bIsFadeIn ? 1.f : 0.f,
		bIsFadeIn ? 0.f : 1.f,
		FadeDuration,
		FadeColor,
		false,
		true
	);
}

void UTransitionControlComponent::Server_NotifyFadeCompleted_Implementation(const int32 DelegateKey)
{
	NotifyFadeCompleted(DelegateKey);
}

void UTransitionControlComponent::StartCameraFade(const bool bIsFadeIn) const
{
	if (FMath::IsNearlyZero(InitialDelay))
	{
		CreateFade(bIsFadeIn);
	}
	else
	{
		FTimerHandle UnusedHandle;
		GetOwner()->GetWorldTimerManager().SetTimer(
			UnusedHandle, FTimerDelegate::CreateWeakLambda(
				this, [this, bIsFadeIn]()
				{
					CreateFade(bIsFadeIn);
				}
			), InitialDelay, false
		);
	}
}
