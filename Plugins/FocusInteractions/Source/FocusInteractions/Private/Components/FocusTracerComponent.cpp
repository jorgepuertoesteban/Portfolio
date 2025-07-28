// Created by Bionic Ape. All rights reserved.

#include "Components/FocusTracerComponent.h"

#include "ActionsSet.h"
#include "FocusInteractions.h"
#include "Net/UnrealNetwork.h"


UFocusTracerComponent::UFocusTracerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	CollisionShape = FCollisionShape::MakeSphere(15);
	SetIsReplicatedByDefault(true);
}

void UFocusTracerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldTrace && GIsClient)
	{
		Trace();
	}

	if (FFocusInteractionsModule::DebugDrawingFTC > 0)
	{
		for (int32 i = ActionsSets.Num() - 1; i >= 0; i--)
		{
			const FString Message = FString::Printf(TEXT("Action %s with index %i: %s"), *ActionsSets[i]->GetActionName().ToString(), i, *GetCurrentActionFriendlyName(i).ToString());
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, Message);
		}
	}
}

void UFocusTracerComponent::BeginPlay()
{
	Super::BeginPlay();
	CollisionShape = FCollisionShape::MakeSphere(TraceSphereRadius);
}

void UFocusTracerComponent::Trace()
{
	FVector StartTrace;
	FVector RotVector;
	GetStartLocationAndForwardVector(StartTrace, RotVector);
	const FVector EndTrace = StartTrace + (RotVector * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoredActors);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (FFocusInteractionsModule::DebugDrawingFTC > 0)
	{
		const FName TraceTag("CollectorTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionParams.TraceTag = TraceTag;
		CollisionParams.bTraceComplex = bTraceComplex;
	}
#endif

	if (bUseLineTrace)
	{
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, FocusableTraceChannel, CollisionParams);
	}
	else
	{
		GetWorld()->SweepSingleByChannel(Hit, StartTrace, EndTrace, FQuat::Identity, FocusableTraceChannel, CollisionShape, CollisionParams);
	}


	if (FFocusInteractionsModule::DebugDrawingFTC > 0)
	{
		if (bUseLineTrace)
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Purple);
		}
		else
		{
			const float SphereRadius = CollisionShape.GetSphereRadius();
			DrawDebugCylinder(GetWorld(), StartTrace, EndTrace, SphereRadius, FMath::CeilToInt(TWO_PI * SphereRadius / 10.f), FColor::Purple);
		}
		DrawDebugSphere(GetWorld(), Hit.Location, 10.f, 12, FColor::Red);
	}

	AActor* ActorHit = Hit.GetActor();

	SetFocusOnActor(ActorHit, Hit.Distance);
}

void UFocusTracerComponent::GetStartLocationAndForwardVector(FVector& StartTrace, FVector& RotVector)
{
	const AController* MyController = GetController();
	const APawn* MyPawn = GetPawn();

	if (MyController && MyPawn)
	{
		FVector CamLoc;
		FRotator CamRot;
		MyController->GetPlayerViewPoint(CamLoc, CamRot);

		IgnoredActors.Empty(); //TODO: change it, It's like this because if the pawn is unpossessed it won't be able to focus it with a new pawn
		IgnoredActors.AddUnique(MyPawn);

		FVector const PawnLocation = MyPawn->GetActorLocation();
		if (bIsThirdPerson)
		{
			StartTrace = PawnLocation;
			CamRot.Pitch = 0;
			RotVector = CamRot.Vector();
		}
		else
		{
			RotVector = CamRot.Vector();
			StartTrace = CamLoc + RotVector * ((PawnLocation - CamLoc) | RotVector);
		}
	}
}

void UFocusTracerComponent::SetOwnerPawn(APawn* NewOwnerPawn, const bool bIsIgnored /*= true*/)
{
	if (OwnerPawn)
	{
		IgnoredActors.Remove(OwnerPawn);
	}

	OwnerPawn = NewOwnerPawn;

	if (bIsIgnored && NewOwnerPawn)
	{
		IgnoredActors.AddUnique(OwnerPawn);
	}
}

void UFocusTracerComponent::OnRep_ShouldTrace()
{
	SetFocusOnActor(nullptr, 0);
}

void UFocusTracerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFocusTracerComponent, bShouldTrace);
}
