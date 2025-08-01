// Created by Jorge Puerto. All Rights Reserved.


#include "Components/JPEWorldRegisterComponent.h"
#include "JPEWorldSubsystem.h"

void UJPEWorldRegisterComponent::BeginPlay()
{
	Super::BeginPlay();

	UJPEWorldSubsystem* Subsystem = UJPEWorldSubsystem::GetJPEWorldSubsystem();

	if (!Subsystem)
	{
		//TODO: LOG
		return;
	}
	Subsystem->RegisterActor(Key, GetOwner());
}
