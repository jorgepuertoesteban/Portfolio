// Created by Bionic Ape. All Rights Reserved.


#include "GameFramework/ActionSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Engine/StaticMeshActor.h"
#include "Net/UnrealNetwork.h"


AActionSequenceActor::AActionSequenceActor(const FObjectInitializer& Init) : Super(Init)
{
	bReplicates = false; //Directly setting bReplicates is the correct procedure for pre-init actors.

	bReplicatePlayback = false;
}

void AActionSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	PlaybackSettings.bAutoPlay = false;
	PlaybackSettings.bHideHud = true;
	PlaybackSettings.FinishCompletionStateOverride = bRestoreStateAtSequenceFinish ? EMovieSceneCompletionModeOverride::ForceRestoreState : EMovieSceneCompletionModeOverride::None;

	ULevelSequencePlayer* LevelSequencePlayer = GetSequencePlayer();
	if (!LevelSequencePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("::BeginPlay() LevelSequencePlayer is null"));
		return;
	}
	LevelSequencePlayer->OnPlay.AddDynamic(this, &AActionSequenceActor::OnSequencePlay);
}

void AActionSequenceActor::AddBindingObjectByTag(const FName& BindingTag, UObject* BindObject)
{
	const UMovieSceneSequence* Sequence = GetSequence();
	ULevelSequencePlayer* LevelSequencePlayer = GetSequencePlayer();

	if (const FMovieSceneObjectBindingIDs* Bindings = Sequence ? Sequence->GetMovieScene()->AllTaggedBindings().Find(BindingTag) : nullptr)
	{
		for (const FMovieSceneObjectBindingID& BindingId : Bindings->IDs)
		{
			if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(BindObject))
			{
				MeshTagBindings.Add(BindingTag, StaticMesh);
			}
			else
			{
				BindingOverrides->AddBinding(BindingId , BindObject);
			}
		}
	}
}

void AActionSequenceActor::OnSequencePlay()
{
	ULevelSequencePlayer* LevelSequencePlayer = GetSequencePlayer();
	if (!LevelSequencePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("ASequenceDirector::OnSequencePlay() LevelSequencePlayer is null"));
		return;
	}

	for (auto MeshTagBind = MeshTagBindings.CreateConstIterator(); MeshTagBind; ++MeshTagBind)
	{
		if (!MeshTagBind)
		{
			continue;
		}
		if (MeshTagBind->Value)
		{
			if (const FMovieSceneObjectBindingIDs* Bindings = LevelSequencePlayer->GetSequence()->GetMovieScene()->AllTaggedBindings().Find(MeshTagBind->Key))
			{
				for (const FMovieSceneObjectBindingID& MovieSceneObjectBindingID : Bindings->IDs)
				{
					TArray<UObject*> BoundObjects = LevelSequencePlayer->GetBoundObjects(MovieSceneObjectBindingID);
					for (UObject* Obj : BoundObjects)
					{
						if (const AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Obj))
						{
							MeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshTagBind->Value);
						}
					}
				}
			}
		}
	}
}

void AActionSequenceActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActionSequenceActor, bRestoreStateAtSequenceFinish);
}
