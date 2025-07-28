// Created by Bionic Ape. All Rights Reserved.


#include "FocusInteractionsWorldSubsystem.h"

void UFocusInteractionsWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	UGameInstance* GameInstance = InWorld.GetGameInstance();
	if (PostProcessActorClass && GameInstance && !GameInstance->IsDedicatedServerInstance())
	{
		AActor* PostProcessActor = InWorld.SpawnActor<AActor>(PostProcessActorClass, FTransform::Identity);
	}
}

void UFocusInteractionsWorldSubsystem::RegisterPointAndClickCamera(APointAndClickCamera* PointAndClickCamera)
{
	PointAndClickCameras.Add(PointAndClickCamera);
}
