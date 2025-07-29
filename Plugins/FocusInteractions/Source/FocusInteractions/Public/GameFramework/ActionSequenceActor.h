// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "ActionSequenceActor.generated.h"

UCLASS()
class FOCUSINTERACTIONS_API AActionSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	TMap<FName, UStaticMesh*> MeshTagBindings;

public:
	
	UPROPERTY(Replicated)
	bool bRestoreStateAtSequenceFinish = true;

public:

	AActionSequenceActor(const FObjectInitializer& Init);

	void AddBindingObjectByTag(const FName& Tag, UObject* BindObject);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnSequencePlay();

protected:

	virtual void BeginPlay() override;

};
