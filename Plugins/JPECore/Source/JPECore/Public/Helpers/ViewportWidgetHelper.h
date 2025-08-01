// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViewportWidgetHelper.generated.h"

class UCameraComponent;
class URotatingMovementComponent;
class UChildActorComponent;

UCLASS()
class JPECORE_API AViewportWidgetHelper : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActor;

public:
	// Sets default values for this actor's properties
	AViewportWidgetHelper();

	UFUNCTION(BlueprintImplementableEvent, Category = "Viewport")
	void OnNewObject(UObject* NewObj);

	UFUNCTION(BlueprintCallable)
	virtual void SetNewObject(UObject* NewObj);

	UFUNCTION(BlueprintCallable)
	virtual AActor* SpawnChildActor(TSubclassOf<AActor> Class);

};
