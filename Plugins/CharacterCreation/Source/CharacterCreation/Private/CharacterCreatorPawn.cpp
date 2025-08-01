// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreatorPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CharacterCreatorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Camera/CameraComponent.h"
#include "Logs/CharacterCreationLog.h"

// Sets default values
ACharacterCreatorPawn::ACharacterCreatorPawn() : Super()
{
	CharacterCreatorComponent = CreateDefaultSubobject<UCharacterCreatorComponent>(TEXT("CharacterCreatorComponent"));
	CharacterCreatorComponent->SetIsReplicated(true);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	RootSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootSkeletalMesh"));
	RootSkeletalMesh->SetupAttachment(GetRootComponent());
	RootSkeletalMesh->SetReceivesDecals(false);
	RootSkeletalMesh->SetCastCapsuleDirectShadow(true);
	RootSkeletalMesh->SetCastCapsuleIndirectShadow(true);
}

void ACharacterCreatorPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents(ChildActorComponents);
	for (UChildActorComponent* CameraComponent : ChildActorComponents)
	{
		for (const FName& Tag : CameraComponent->ComponentTags)
		{
			if (CamerasMap.Contains(Tag))
			{
				UE_LOG(CharacterCreationLog, Error, TEXT("A Camera Component with the tag [%s] already exists"), *Tag.ToString());
				continue;
			}
			CamerasMap.Add(Tag, CameraComponent->GetChildActor());
		}
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("Controller is not a APlayerController"));
		return;		
	}

	ControlComponent = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(PC);
	if (!ControlComponent)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("Controller does not have a UCharacterCreatorControlComponent Component"));
		return;
	}

	ControlComponent->ShowCharacterCreatorHUD();
	ShowCamera(TEXT("Camera_Default"));
	PlayerCameraManager = PC->PlayerCameraManager;
	if (bSyncWithControlComponent)
	{
		CharacterCreatorComponent->SetCharacterCreator(ControlComponent->GetCharacterCreator());
		ControlComponent->OnNewMainCharacterCreatorChanged().AddWeakLambda(this, [this](UCharacterCreator* NewCharacterCreator)
		{
			CharacterCreatorComponent->SetCharacterCreator(NewCharacterCreator);
		});
	}
}

void ACharacterCreatorPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	if (ControlComponent)
	{
		ControlComponent->OnNewMainCharacterCreatorChanged().RemoveAll(this);
		ControlComponent->HideCharacterCreatorHUD();
	}
}

void ACharacterCreatorPawn::ShowCamera(const FName& CameraTag)
{
	if (CamerasMap.Contains(CameraTag) && PlayerCameraManager)
	{
		static const FViewTargetTransitionParams TransitionParams = []
		{
			FViewTargetTransitionParams Params;
			Params.BlendTime = 1.f;
			Params.BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;
			return Params;
		}(); //static immediate invocation to save some performance
		PlayerCameraManager->SetViewTarget(CamerasMap[CameraTag], TransitionParams);
	}
}
