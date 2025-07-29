// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionSequenceTypes.h"
#include "ActionSequenceDirector.generated.h"

class AActionSequenceActor;
class USceneComponent;

UCLASS()
class FOCUSINTERACTIONS_API AActionSequenceDirector : public AActor
{
	GENERATED_BODY()

protected:

	static float DestroyLifespan;

public:

FOnActionSequenceFinishedDelegate OnActionSequenceFinishedDelegate;

	UPROPERTY(EditAnywhere, Replicated, Category = "Action")
	TArray<FActionSequenceBindingTag> ActionSequenceBindingTag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Action")
	USceneComponent* RootComp;
	 
	UPROPERTY(EditAnywhere, Replicated, Category = "Action")
	FActionSequenceBindingParams ActionSequenceBindingParams;

	UPROPERTY(Transient)
	AActionSequenceActor* SequenceActor;

	bool bRestoreStateAtSequenceFinish = true;

public:	
	AActionSequenceDirector();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnSequenceFinished();

protected:

	virtual void BeginPlay() override;
	
};
