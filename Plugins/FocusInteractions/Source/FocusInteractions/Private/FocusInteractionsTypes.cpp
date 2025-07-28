//Created by Bionic Ape. All Rights Reserved.

#include "FocusInteractionsTypes.h"

#include "ActionsSet.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Components/FocusableComponent.h"
#include "ActionableType.h"
#include "ActionType.h"

FString FFocusPerformAction::ToString() const
{
	return FString::Printf(
		TEXT("ActionsSet:%s, ActionType:%s, ActionableType:%s, Focusable:%s, ActionPawn:%s, ActionController:%s, FocusedActor:%s, Distance:%f"),
		ActionsSet ? *ActionsSet->GetActionName().ToString() : TEXT("nullptr"),
		ActionType ? *ActionType->GetActionName().ToString() : TEXT("nullptr"),
		ActionableType ? *ActionableType->GetActionName().ToString() : TEXT("nullptr"),
		Focusable ? *Focusable->GetName() : TEXT("nullptr"),
		ActionPawn ? *ActionPawn->GetName() : TEXT("nullptr"),
		ActionController ? *ActionController->GetName() : TEXT("nullptr"),
		FocusedActor ? *FocusedActor->GetName() : TEXT("nullptr"),
		Distance
	);
}

void FFocusPerformAction::Reset()
{
	ActionsSet = nullptr;
	ActionType = nullptr;
	ActionableType = nullptr;
	Focusable = nullptr;
	ActionPawn = nullptr;
	ActionController = nullptr;
	Distance = 0.0f;
	FocusedActor = nullptr;
}
