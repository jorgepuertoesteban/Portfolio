// Created by Jorge Puerto. All Rights Reserved.


#include "Camera/TransitionCameraActor.h"
#include "JPEWorldSubsystem.h"

void ATransitionCameraActor::BeginPlay()
{
	Super::BeginPlay();


	UJPEWorldSubsystem* Subsystem = UJPEWorldSubsystem::GetJPEWorldSubsystem();

	if (!Subsystem)
	{
		//TODO: LOG
		return;
	}
	Subsystem->RegisterTransitionCamera(Key, this);
}
