// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/CCViewportRendererActor.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ChildActorComponent.h"
#include "Components/CharacterCreatorComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACCViewportRendererActor::ACCViewportRendererActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");


	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh0"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	OpacityCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>("OpacityCaptureComponent2D");
	OpacityCaptureComponent2D->SetupAttachment(GetCaptureComponent2D());

	CharacterCreatorComponent = CreateDefaultSubobject<UCharacterCreatorComponent>("CharacterCreatorComponent");
	CharacterCreatorComponent->SetRootSkeletalMesh(SkeletalMeshComponent);
}

void ACCViewportRendererActor::SetOpacityRenderTarget(UTextureRenderTarget2D* OpacityRenderTarget)
{
	OpacityCaptureComponent2D->TextureTarget = OpacityRenderTarget;
}

void ACCViewportRendererActor::SetRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
	GetCaptureComponent2D()->TextureTarget = RenderTarget;
}

void ACCViewportRendererActor::SetCharacterCreator(UCharacterCreator* NewCharacterCreator)
{
	CharacterCreatorComponent->SetCharacterCreator(NewCharacterCreator);
}


