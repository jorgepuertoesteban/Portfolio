// Created by Jorge Puerto. All Rights Reserved.


#include "Helpers/ViewportWidgetHelper.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AViewportWidgetHelper::AViewportWidgetHelper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(-50.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate.Yaw = 40.f;
	
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	ChildActor->SetupAttachment(RootComponent);
}



void AViewportWidgetHelper::SetNewObject(UObject* NewObj)
{
	OnNewObject(NewObj);
}

AActor* AViewportWidgetHelper::SpawnChildActor(TSubclassOf<AActor> Class)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		//TODO: LOG
		return nullptr;
	}

	ChildActor->SetChildActorClass(Class);
	return ChildActor->GetChildActor();
}


