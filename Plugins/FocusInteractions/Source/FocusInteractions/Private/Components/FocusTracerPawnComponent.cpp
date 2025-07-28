// Created by Bionic Ape. All Rights Reserved.


#include "Components/FocusTracerPawnComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActionsSet.h"
#include "FocusInteractionsTypes.h"
#include "Components/FocusableComponent.h"
#include "ActionType.h"

void UFocusTracerPawnComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.bCanEverTick = true;
	SetOwnerPawn(CastChecked<APawn>(GetOwner()));
}