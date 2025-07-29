// Created by Jorge Puerto. All rights reserved.

#include "Components/FocusComponent.h"
#include "ActionsSet.h"
#include "ActionType.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Components/FocusableComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "FocusInteractionsTypes.h"
#include "FocusInteractionsLog.h"
#include "FocusableFilter.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/FocusComponentOwner.h"


UFocusComponent::UFocusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

UFocusComponent* UFocusComponent::GetFocusComponent(const AActor* Owner)
{
	if (!Owner)
	{
		return nullptr;
	}
	if (Owner->Implements<UFocusComponentOwner>())
	{
		return IFocusComponentOwner::Execute_GetFocusComponent(Owner);
	}
	return Cast<UFocusComponent>(Owner->GetComponentByClass(UFocusComponent::StaticClass()));
}

void UFocusComponent::BeginPlay()
{
	Super::BeginPlay();

	ActionsSets.Remove(nullptr); //We Remove all nullptr from ActionsSets

	if (bAutoDiscoverOwner)
	{
		bool bIsInitialized = false;

		if (GetOwner()->IsA(AController::StaticClass()))
		{
			OwnerController = Cast<AController>(GetOwner());
			bIsInitialized = true;
		}
		else if (GetOwner()->IsA(APawn::StaticClass()))
		{
			SetOwnerPawn(Cast<APawn>(GetOwner()));
			bIsInitialized = true;
		}
		if (!bIsInitialized)
		{
			UE_LOG(FocusInteractionsLog, Log, TEXT("UFocusComponent::BeginPlay: bAutoDiscoverOwner is set to true but the owner is neither a Pawn nor a Controller"));
		}
	}

	if (!IsLocallyControlled())
	{
		Deactivate(); //I don't know if we will need to use it remotely
	}
}

void UFocusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (FocusInfo.FocusedComponent)
	{
		FocusInfo.FocusedComponent->EndFocus(this);

		OnEndFocus.Broadcast(FocusInfo.FocusedComponent);
	}
}

void UFocusComponent::SetOwnerController(AController* Controller)
{
	OwnerController = Controller;
}

void UFocusComponent::SetOwnerPawn(APawn* NewOwnerPawn, const bool bIsIgnored /*= true*/)
{
	OwnerPawn = NewOwnerPawn;
}

APawn* UFocusComponent::GetOwnerPawn() const
{
	return OwnerPawn;
}

void UFocusComponent::Deactivate()
{
	if (FocusInfo.FocusedComponent)
	{
		FocusInfo.FocusedComponent->EndFocus(this);
	}
	FocusInfo.Reset();
	Super::Deactivate();
}

bool UFocusComponent::MakeInteractionParams(int32 const InteractionsSetIndex, FFocusActionParams& Params)
{
	if (FocusInfo.FocusedComponent && ActionsSets.IsValidIndex(InteractionsSetIndex))
	{
		UActionType* NewActionType = ActionsSets[InteractionsSetIndex]->GetActionType(FocusInfo.FocusedComponent->ActionableType, FocusInfo.Distance);
		bool const bCanPerformInteraction = NewActionType != nullptr;

		if (bCanPerformInteraction)
		{
			Params.CallerActor = GetOwner();
			Params.CallerFocus = this;
			Params.Action = NewActionType;
			Params.ReceiverActor = FocusInfo.FocusedActor;
			Params.ReceiverFocused = FocusInfo.FocusedComponent;
		}

		return bCanPerformInteraction;
	}
	return false;
}

AController* UFocusComponent::GetController() const
{
	return OwnerController ? OwnerController : OwnerPawn ? OwnerPawn->GetController() : nullptr;
}

AController* UFocusComponent::GetActionController() const
{
	return OwnerController ? OwnerController : OwnerPawn ? OwnerPawn->GetController() : nullptr;
}

APawn* UFocusComponent::GetPawn() const
{
	return OwnerPawn ? OwnerPawn : GetController()->GetPawn().Get();
}

APawn* UFocusComponent::GetActionPawn() const
{
	return OwnerPawn ? OwnerPawn : GetActionController()->GetPawn().Get();
}

FName UFocusComponent::GetCurrentActionFriendlyName(int32 const InteractionsSetIndex) const
{
	if (FocusInfo.FocusedComponent && ActionsSets.IsValidIndex(InteractionsSetIndex))
	{
		if (const UActionType* NewActionType = ActionsSets[InteractionsSetIndex]->GetActionType(FocusInfo.FocusedComponent->ActionableType, FocusInfo.Distance))
		{
			return NewActionType->GetActionName();
		}
	}
	return NAME_None;
}

void UFocusComponent::GetFriendlyNames(TArray<FFocusActionFriendlyName>& OutFriendlyNames) const
{
	if (FocusInfo.FocusedComponent)
	{
		for (const UActionsSet* ActionSet : ActionsSets)
		{
			if (ActionSet)
			{
				if (const UActionType* NewActionType = ActionSet->GetActionType(FocusInfo.FocusedComponent->ActionableType, FocusInfo.Distance))
				{
					OutFriendlyNames.Emplace(FFocusActionFriendlyName(NewActionType->GetActionName(), true));
				}
				else
				{
					OutFriendlyNames.Emplace(FFocusActionFriendlyName(NAME_None, false));
				}
			}
		}
	}
	else
	{
		for (UActionsSet* ActionSet : ActionsSets)
		{
			OutFriendlyNames.Emplace(NAME_None, false);
		}
	}
}

void UFocusComponent::SetFocusOnActor(AActor* ActorToFocus, float Distance)
{
	FFocusInfo NewInfo;
	NewInfo.FocusComponent = this;
	if (ActorToFocus)
	{
		if (UFocusableComponent* FocusableComponent = UFocusableComponent::GetFocusableComponent(ActorToFocus))
		{
			if (!IsAllowedByFilters(ActorToFocus, FocusableComponent))
			{
				//TODO: LOG?
				return;
			}

			int32 Index = 0;

			float const NewDistance = bForceCloseDistance ? 1.f : Distance;

			for (UActionsSet* ActionsSet : ActionsSets)
			{
				if (UActionType* NewAction = ActionsSet->GetActionType(FocusableComponent->ActionableType, NewDistance))
				{
					if (NewAction->CanExecuteAction(GetOwner(), ActorToFocus))
					{
						NewInfo.Actions.Emplace(ActionsSet, NewAction, Index, ActorToFocus);
					}
				}
				Index++;
			}
			if (NewInfo.Actions.Num() > 0)
			{
				NewInfo.Distance = NewDistance;
				NewInfo.FocusedActor = ActorToFocus;
				NewInfo.FocusedComponent = FocusableComponent;
			}
		}
	}

	if (FocusInfo != NewInfo)
	{
		if (FocusInfo.FocusedComponent)
		{
			FocusInfo.FocusedComponent->EndFocus(this);

			for (const FActionByActionsSetResult& Result : FocusInfo.Actions)
			{
				if (Result.Action)
				{
					Result.Action->EndFocus(this, Result);
				}
			}
			
			OnEndFocus.Broadcast(FocusInfo.FocusedComponent);
		}
		if (NewInfo.FocusedComponent)
		{
			FocusInfo = NewInfo; //This will be updated anyway but we need to update it now because it will occur a access memory violation otherwise calling OnNewFocus.Broadcast(Info)
			for (const FActionByActionsSetResult& Result : FocusInfo.Actions)
			{
				if (Result.Action)
				{
					Result.Action->StartFocus(this, Result);
				}
			}
			FocusInfo.FocusedComponent->StartFocus(this);
			OnNewFocus.Broadcast(FocusInfo);
		}
	}

	FocusInfo = NewInfo; //We always update with new parameters, == don't track all parameters
}

UActionsSet* UFocusComponent::GetActionsSet(int32 Index) const
{
	if (ActionsSets.IsValidIndex(Index))
	{
		return ActionsSets[Index];
	}
	UE_LOG(FocusInteractionsLog, Log, TEXT("UFocusComponent::GetActionSet: Index is not valid!"));
	return nullptr;
}

bool UFocusComponent::GetPerformActionWithIndex(int32 actionsSetIndex, FFocusPerformAction& FocusPerformAction) const
{
	const bool bIsValidIndex = ActionsSets.IsValidIndex(actionsSetIndex);
	UActionsSet* DebugActionSet = GetActionsSet(actionsSetIndex);
	const bool bGetPerformAction = GetPerformAction(DebugActionSet, FocusPerformAction);

	return bIsValidIndex ? bGetPerformAction : false; //TODO Uncomment

	//return ActionsSets.IsValidIndex(actionsSetIndex) ? GetPerformAction(GetActionsSet(actionsSetIndex), FocusPerformAction) : false; //TODO Uncomment
}

bool UFocusComponent::GetPerformAction(UActionsSet* ActionSet, FFocusPerformAction& FocusPerformAction) const
{
	if (ActionSet && FocusInfo.FocusedComponent)
	{
		FocusPerformAction.ActionType = ActionSet->GetActionType(FocusInfo.FocusedComponent->ActionableType, FocusInfo.Distance);
		if (FocusPerformAction.ActionType)
		{
			FocusPerformAction.ActionableType = FocusInfo.FocusedComponent->ActionableType;
			FocusPerformAction.ActionsSet = ActionSet;
			FocusPerformAction.ActionPawn = GetActionPawn();
			FocusPerformAction.ActionController = GetActionController();
			FocusPerformAction.Distance = FocusInfo.Distance;
			FocusPerformAction.Focusable = FocusInfo.FocusedComponent;
			FocusPerformAction.FocusedActor = FocusInfo.FocusedActor;
			return true;
		}
	}
	return false;
}

void UFocusComponent::Server_SetNewActionsSetDeleteOlds_Implementation(TArray<UActionsSet*> const& NewActionSet)
{
	ActionsSets.Empty();
	if (!NewActionSet.IsEmpty())
	{
		//ActionsSets.Add(NewActionSet);
		ActionsSets.Append(NewActionSet);
		OnNewActionsSets.Broadcast();
	}
}

void UFocusComponent::SetNewActionsSetDeleteOlds(TArray<UActionsSet*> NewActionSet)
{
	Server_SetNewActionsSetDeleteOlds(NewActionSet);
}

void UFocusComponent::SetActionsSet(UActionsSet* NewActionSet, const int32 Index)
{
	Server_SetActionsSet(NewActionSet, Index);
}

void UFocusComponent::Server_SetActionsSet_Implementation(UActionsSet* NewActionSet, const int32 Index)
{
	// If index is valid or adding to the end, proceed to set or add the action set
	if (ActionsSets.IsValidIndex(Index))
	{
		// If the index is valid, directly set the NewActionSet
		ActionsSets[Index] = NewActionSet;
	}
	else
	{
		ActionsSets.Add(NewActionSet);
	}
}

bool UFocusComponent::IsAllowedByFilters(AActor* Actor, UFocusableComponent* FocusableComponent) const
{
	for (const UFocusableFilter* Filter : Filters)
	{
		if (!Filter->IsAllowed(this, Actor, FocusableComponent))
		{
			return false;
		}
	}
	return true;
}

bool UFocusComponent::CanExecuteAnyAction(const TArray<UActionsSet*>& InActionsSets, UActionableType* ActionableType, AActor* ActionActor, AActor* ActionableActor) const
{
	for (const UActionsSet* ActionsSet : InActionsSets)
	{
		if (const UActionType* ActionType = ActionsSet->GetActionType(ActionableType))
		{
			if (ActionType && ActionType->CanExecuteAction(ActionActor, ActionableActor))
			{
				return true;
			}
		}
	}
	return false;
}

bool UFocusComponent::PerformAction(int32 ActionsSetIndex)
{
	FFocusPerformAction FocusPerformAction;
	if (GetPerformActionWithIndex(ActionsSetIndex, FocusPerformAction))
	{
		if (FocusPerformAction.ActionType->IsExecutedOnServer())
		{
			ServerPerformAction(FocusPerformAction); //Use Client focus
			return true;
		}
		else
		{
			return PerformActionType(FocusPerformAction);
		}
	}
	return false;
}

bool UFocusComponent::PerformActionAgainstActor(UActionType* Action, AActor* ActorToFocus, float Distance)
{
	if (!ActorToFocus)
	{
		return false;
	}
	if (!Action)
	{
		return false;
	}
	SetFocusOnActor(ActorToFocus, Distance);
	return PerformThisAction(Action);
}

bool UFocusComponent::PerformThisAction(UActionType* Action)
{
	if (!Action)
	{
		return false;
	}

	for (const FActionByActionsSetResult ActionByActionsSetResult : FocusInfo.Actions)
	{
		if (ActionByActionsSetResult.Action == Action)
		{
			return PerformAction(ActionByActionsSetResult.ActionSetIndex);
		}
	}
	return false;
}


bool UFocusComponent::PerformActionWithFocusPerformAction(FFocusPerformAction FocusPerformAction)
{
	if (!FocusPerformAction.ActionType)
	{
		return false;
	}
	if (FocusPerformAction.ActionType->IsExecutedOnServer())
	{
		ServerPerformAction(FocusPerformAction); //Use Client focus
		return true;
	}
	else
	{
		return PerformActionType(FocusPerformAction);
	}
}

bool UFocusComponent::PerformActionWithClientFocus(int32 ActionsSetIndex)
{
	FFocusPerformAction FocusPerformAction;
	if (GetPerformActionWithIndex(ActionsSetIndex, FocusPerformAction))
	{
		ServerPerformAction(FocusPerformAction);
		return true;
	}
	return false;
}

bool UFocusComponent::IsLocallyControlled() const
{
	const ENetMode NetMode = GetNetMode();

	if (NetMode == NM_Standalone)
	{
		// Not networked.
		return true;
	}

	if (const AActor* Owner = GetOwner())
	{
		if (NetMode == NM_Client && Owner->GetLocalRole() == ROLE_AutonomousProxy)
		{
			// Networked client in control.
			return true;
		}

		if (Owner->GetRemoteRole() != ROLE_AutonomousProxy && Owner->GetLocalRole() == ROLE_Authority)
		{
			// Local authority in control.
			return true;
		}
	}

	return false;
}

void UFocusComponent::ServerPerformAction_Implementation(FFocusPerformAction FocusPerformAction)
{
	(void)PerformActionType(FocusPerformAction);
}

bool UFocusComponent::PerformActionType(FFocusPerformAction FocusPerformAction) const
{
	bool bPerformed = FocusPerformAction.ActionType->PerformActionType(FocusPerformAction);
	if (bPerformed)
	{
		OnActionPerformed.Broadcast(FocusPerformAction);
	}
	return bPerformed;
}


bool UFocusComponent::ServerPerformAction_Validate(FFocusPerformAction FocusPerformAction)
{
	//TODO: DO more validation
	//bool const bIsSameActionPawn = FocusPerformAction.ActionPawn == GetActionPawn();
	return true;
}

void UFocusComponent::OnRep_ActionsSets()
{
	OnNewActionsSets.Broadcast();
}

void UFocusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFocusComponent, ActionsSets);
}
