 //Created by Bionic Ape. All rights reserved.

#pragma once

#include "Components/FocusComponent.h"
#include "WorldCollision.h"
#include "FocusTracerComponent.generated.h"

class AActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FOCUSINTERACTIONS_API UFocusTracerComponent : public UFocusComponent
{
	GENERATED_BODY()

public:

	/*static */FCollisionShape CollisionShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	TEnumAsByte<ECollisionChannel> FocusableTraceChannel = ECC_Visibility;

	float TraceDistance = 100000.f;

	UPROPERTY(ReplicatedUsing="OnRep_ShouldTrace", EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bShouldTrace = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bTraceComplex = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	TArray<const AActor*> IgnoredActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bUseLineTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	float TraceSphereRadius = 15.f;

public:

	UFocusTracerComponent();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_ShouldTrace();

public:

	//UActorComponent Interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//End UActorComponent Interface

	virtual void Trace();

	virtual void GetStartLocationAndForwardVector(FVector& StartTrace, FVector& RotVector);

	virtual void SetOwnerPawn(APawn* NewOwnerPawn, const bool bIsIgnored = true) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
