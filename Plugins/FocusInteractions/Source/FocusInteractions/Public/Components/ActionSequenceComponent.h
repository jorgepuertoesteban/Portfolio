// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionSequenceTypes.h"
#include "ActionSequenceComponent.generated.h"

class UActionSequences;
class ACharacter;
class ULevelSequence;
class AActionSequenceActor;
class AActionSequenceDirector;
class UActionType;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOCUSINTERACTIONS_API UActionSequenceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActionSequenceComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UActionSequenceComponent* GetActionSequenceComponent(AActor* Actor);

	virtual bool PerformActionSequence(UActionType* Action, ACharacter* Character, FOnActionSequenceFinishedDelegate& OnActionSequenceFinishedDelegate);

	virtual ULevelSequence* SequenceFromAction(UActionType* Action);

	UFUNCTION(BlueprintCallable)
	void ActionSequenceEvent();

	UFUNCTION(BlueprintCallable)
	void AbortSequence();

public:

	FOnActionSequenceEventDelegate ActionSequenceEventDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Action)
	UActionSequences* ActionSequences;
	
	UPROPERTY()
	AActor* SequenceRootActor;	

protected:

	UPROPERTY(Transient)
	AActionSequenceDirector* Director;
};
