// Created by Jorge Puerto. All Rights Reserved.

#include "Components/ActionSequenceComponent.h"
#include "LevelSequence.h"
#include "ActionSequences.h"
#include "GameFramework/ActionSequenceDirector.h"
#include "GameFramework/Character.h"
#include "Components/FocusComponent.h"
#include "Components/FocusableComponent.h"
#include "ActionType_SequenceBase.h"
#include "Interfaces/ActionSequenceComponentOwner.h"

UActionSequenceComponent::UActionSequenceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UActionSequenceComponent* UActionSequenceComponent::GetActionSequenceComponent(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}
	if (IActionSequenceComponentOwner* ActionSequenceComponentOwner = Cast<IActionSequenceComponentOwner>(Actor))
	{
		return ActionSequenceComponentOwner->GetActionSequenceComponent_Implementation();
	}
	if (UActionSequenceComponent* ActionSequenceComponent = Cast<UActionSequenceComponent>(Actor->GetComponentByClass(UActionSequenceComponent::StaticClass())))
	{
		return ActionSequenceComponent;
	}
	return nullptr;
}

bool UActionSequenceComponent::PerformActionSequence(UActionType* Action, ACharacter* Character, FOnActionSequenceFinishedDelegate& OnActionSequenceFinishedDelegate)
{
	if (!Action)
	{
		return false;
	}

	if (!Character)
	{
		return false;
	}

	UFocusComponent* CharacterFocusComponent = UFocusComponent::GetFocusComponent(Character);
	if (!CharacterFocusComponent)
	{
		return false;
	}

	CharacterFocusComponent->SetFocusOnActor(GetOwner(), GetOwner()->GetDistanceTo(Character));
	bool bValidAction = false;
	for (auto FocusAction : CharacterFocusComponent->FocusInfo.Actions)
	{
		if (FocusAction.Action == Action)
		{
			bValidAction = true;
			break;
		}
	}
	if (!bValidAction)
	{
		return false;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}

	TArray<FActionSequenceBindingTag> ActionSequenceBindingTags;

	FActionSequenceBindingTag CharacterTag;
	CharacterTag.Tag = FName("C");
	CharacterTag.Object = Character;
	ActionSequenceBindingTags.Add(CharacterTag);


	FActionSequenceBindingTag ComponentTag;
	ComponentTag.Tag = FName("SO");
	ComponentTag.Object = GetOwner();
	ActionSequenceBindingTags.Add(ComponentTag);

	FActionSequenceBindingParams Params;
	Params.Sequence = SequenceFromAction(Action);
	Params.SequenceRoot = GetOwner()->GetActorTransform();
	Params.ActionSequenceBindingTags = ActionSequenceBindingTags;

	Director = World->SpawnActorDeferred<AActionSequenceDirector>(AActionSequenceDirector::StaticClass(), Params.SequenceRoot);
	Director->OnActionSequenceFinishedDelegate = OnActionSequenceFinishedDelegate;
	Director->ActionSequenceBindingParams = Params;

	if (const UActionType_SequenceBase* ActionSequence = Cast<UActionType_SequenceBase>(Action))
	{
		Director->bRestoreStateAtSequenceFinish = ActionSequence->bRestoreStateAtSequenceFinish;
	}

	Director->FinishSpawning(Params.SequenceRoot);

	//Weak Pointers
	TWeakObjectPtr<UFocusComponent> WeakCharacterFocusComponent = CharacterFocusComponent;
	TWeakObjectPtr<UActionType> WeakAction = Action;
	//End Weak Pointers
	
	ActionSequenceEventDelegate.BindWeakLambda(this, [this, WeakCharacterFocusComponent, WeakAction]
	{
		if (WeakCharacterFocusComponent.IsValid() && WeakAction.IsValid())
		{
			WeakCharacterFocusComponent->SetFocusOnActor(GetOwner(), GetOwner()->GetDistanceTo(WeakCharacterFocusComponent->GetOwner()));
			WeakCharacterFocusComponent->PerformThisAction(WeakAction.Get());			
		}
	});

	return true;
}

ULevelSequence* UActionSequenceComponent::SequenceFromAction(UActionType* Action)
{
	int NumberOfSequences = ActionSequences->ActionSequences[Action].Sequences.Num();
	return ActionSequences->ActionSequences[Action].Sequences[FMath::RandRange(0, NumberOfSequences - 1)].LoadSynchronous();
}

void UActionSequenceComponent::ActionSequenceEvent()
{
	ActionSequenceEventDelegate.ExecuteIfBound();
}


void UActionSequenceComponent::AbortSequence()
{
	if (!Director)
	{
		return;
	}
	Director->Destroy(true);
}
