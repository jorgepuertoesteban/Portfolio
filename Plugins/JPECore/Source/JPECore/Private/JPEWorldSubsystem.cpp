// Created by Jorge Puerto. All Rights Reserved.


#include "JPEWorldSubsystem.h"

#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

UJPEWorldSubsystem* UJPEWorldSubsystem::Instance = nullptr;

UJPEWorldSubsystem* UJPEWorldSubsystem::GetJPEWorldSubsystem(AActor* Actor)
{
	if (!Actor || !Actor->GetWorld())
	{
		return nullptr;
	}

	return Actor->GetWorld()->GetSubsystem<UJPEWorldSubsystem>();
}

bool UJPEWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer) && bShouldCreateSubsystem;
}

void UJPEWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Instance = this;
}

void UJPEWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Instance = nullptr;
}

AActor* UJPEWorldSubsystem::GetRegisteredCameraActors(const FString& TransitionCameraString)
{
	if (TransitionCameraActors.Contains(TransitionCameraString))
	{
		//TODO: log this
		return TransitionCameraActors[TransitionCameraString];
	}
	return nullptr;
}

void UJPEWorldSubsystem::RegisterTransitionCamera(const FString& TransitionCameraString, AActor* TransitionCamera)
{
	TransitionCameraActors.Add(TransitionCameraString, TransitionCamera);
}

void UJPEWorldSubsystem::RegisterActor(const FString& Key, AActor* Actor)
{
	ActorsByKey.Add(Key,Actor);
}

void UJPEWorldSubsystem::SetViewTargetForTransitionCameraByString(const FString& TransitionCameraString, APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		//TODO: log this
		return;
	}

	if (!TransitionCameraActors.Contains(TransitionCameraString))
	{
		//TODO: log this
		return;
	}

	PlayerController->SetViewTargetWithBlend(TransitionCameraActors[TransitionCameraString], 2.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 2.f, false);
}
