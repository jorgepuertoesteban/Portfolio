// Created by Jorge Puerto. All Rights Reserved.


#include "GameFramework/ActionSequenceDirector.h"
#include "GameFramework/ActionSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Net/UnrealNetwork.h"

float AActionSequenceDirector::DestroyLifespan = .1f;


AActionSequenceDirector::AActionSequenceDirector()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	bReplicates = true;
}

void AActionSequenceDirector::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActionSequenceDirector, ActionSequenceBindingParams);
}

void AActionSequenceDirector::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("AActionSequenceDirector::BeginPlay World is null"));
		return;
	}

	SequenceActor = World->SpawnActorDeferred<AActionSequenceActor>(AActionSequenceActor::StaticClass(), ActionSequenceBindingParams.SequenceRoot);
	if (!SequenceActor)
	{
		UE_LOG(LogTemp, Error, TEXT("AActionSequenceDirector::BeginPlay SequenceActor is null"));
		return;
	}
	SequenceActor->SetSequence(ActionSequenceBindingParams.Sequence);
	SequenceActor->bOverrideInstanceData = true;
	SequenceActor->bRestoreStateAtSequenceFinish = bRestoreStateAtSequenceFinish;

	UDefaultLevelSequenceInstanceData* DefaultInstanceData = Cast<UDefaultLevelSequenceInstanceData>(SequenceActor->DefaultInstanceData);
	if (!DefaultInstanceData)
	{
		UE_LOG(LogTemp, Error, TEXT("AActionSequenceDirector::BeginPlay DefaultInstanceData is null"));
		return;
	}
	DefaultInstanceData->TransformOrigin = ActionSequenceBindingParams.SequenceRoot;

	SequenceActor->FinishSpawning(ActionSequenceBindingParams.SequenceRoot);
	SequenceActor->ResetBindings();



	ULevelSequencePlayer* LevelSequencePlayer = SequenceActor->GetSequencePlayer();
	if (!LevelSequencePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("AActionSequenceDirector::BeginPlay LevelSequencePlayer is null"));
		return;
	}

	LevelSequencePlayer->OnFinished.AddDynamic(this, &AActionSequenceDirector::OnSequenceFinished);


	for (const FActionSequenceBindingTag& SequenceBindingTag : ActionSequenceBindingParams.ActionSequenceBindingTags)
	{
		SequenceActor->AddBindingObjectByTag(SequenceBindingTag.Tag, SequenceBindingTag.Object);
	}

	LevelSequencePlayer->Play();
}

void AActionSequenceDirector::OnSequenceFinished()
{
	if (GetLocalRole() == ROLE_Authority || GEngine->GetNetMode(GWorld) == NM_Standalone)
	{
		SetLifeSpan(AActionSequenceDirector::DestroyLifespan);
	}

	OnActionSequenceFinishedDelegate.ExecuteIfBound();
}

void AActionSequenceDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SequenceActor)
	{
		SequenceActor->Destroy();
	}
	Super::EndPlay(EndPlayReason);
}
