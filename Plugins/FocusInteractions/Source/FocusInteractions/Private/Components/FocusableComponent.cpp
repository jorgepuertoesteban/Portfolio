// Created by Jorge Puerto. All rights reserved.

#include "Components/FocusableComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Controller.h"
#include "Components/FocusComponent.h"
#include "Interfaces/FocusableComponentOwner.h"
#include "Net/UnrealNetwork.h"

UFocusableComponent::UFocusableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFocusableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFocusableComponent, ActionableType);
}

UFocusableComponent* UFocusableComponent::GetFocusableComponent(AActor* Actor)
{
	if (!Actor || !Actor->Implements<UFocusableComponentOwner>())
	{
		if (UFocusableComponent* FocusableComponent = Cast<UFocusableComponent>(Actor->GetComponentByClass(UFocusableComponent::StaticClass())))
		{
			return FocusableComponent;
		}
		return nullptr;
	}
	return IFocusableComponentOwner::Execute_GetFocusableComponent(Actor);
}

void UFocusableComponent::StartFocus(UFocusComponent const* const FocusComponent)
{
	SetRenderCustomDepth();
	if (FocusComponent)
	{
		OnStartFocusDelegate.Broadcast(FocusComponent->FocusInfo);
	}
}

void UFocusableComponent::EndFocus(UFocusComponent const* const FocusComponent)
{
	if (Meshes.Num() > 0)
	{
		for (TWeakObjectPtr<UMeshComponent> EachComponent : Meshes)
		{
			if (EachComponent.IsValid())
			{
				EachComponent->SetRenderCustomDepth(false);
			}
		}
	}
	Meshes.Empty();
	OnEndFocusDelegate.Broadcast();
}

void UFocusableComponent::SetRenderCustomDepth()
{
	TArray<UMeshComponent*> MeshesAux;
	if (AActor* Owner = GetOwner())
	{
		Owner->GetComponents<UMeshComponent>(MeshesAux);
		for (UMeshComponent* EachComponent : MeshesAux)
		{
			Meshes.AddUnique(EachComponent);
			EachComponent->SetRenderCustomDepth(true);
			EachComponent->SetCustomDepthStencilValue(StencilFocusValue);
		}
	}
}

void UFocusableComponent::ExecuteAction(const FFocusActionParams& Params)
{
	OnRequestExecuteAction.Broadcast(Params);
}
