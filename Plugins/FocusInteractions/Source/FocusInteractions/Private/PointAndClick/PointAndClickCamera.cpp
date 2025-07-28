// Created by Bionic Ape. All Rights Reserved.

#include "PointAndClick/PointAndClickCamera.h"
#include "FocusInteractionsWorldSubsystem.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

APointAndClickCamera::APointAndClickCamera() :Super()
{
	ShapeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShapeCollision"));
	ShapeCollision->SetAbsolute(false, true, false);
	ShapeCollision->SetupAttachment(GetRootComponent());


	InitializationDetectionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InitializationDetectionCollision"));
	InitializationDetectionCollision->SetAbsolute(false, true, false);
	InitializationDetectionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InitializationDetectionCollision->SetupAttachment(GetRootComponent());
	
	GetCameraComponent()->SetConstraintAspectRatio(false);
}

void APointAndClickCamera::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!bAbsoluteBoxRotation)
	{
		InitializationDetectionCollision->SetAbsolute(false, false, false);
		ShapeCollision->SetAbsolute(false, false, false);
	}
}

void APointAndClickCamera::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		UFocusInteractionsWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UFocusInteractionsWorldSubsystem>();
		WorldSubsystem->RegisterPointAndClickCamera(this);
	}
}

