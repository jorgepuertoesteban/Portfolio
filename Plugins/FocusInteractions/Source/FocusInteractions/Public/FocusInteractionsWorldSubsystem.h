// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FocusInteractionsWorldSubsystem.generated.h"

class APointAndClickCamera;
/**
 * 
 */
UCLASS(Config = Focus)
class FOCUSINTERACTIONS_API UFocusInteractionsWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
protected:

	UPROPERTY(Config)
	TSubclassOf<AActor> PostProcessActorClass;

	UPROPERTY(Transient)
	TSet<APointAndClickCamera*> PointAndClickCameras; 

public:	

	/** Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void RegisterPointAndClickCamera(APointAndClickCamera* PointAndClickCamera);

	const TSet<APointAndClickCamera*>& GetPointAndClickCameras() const
	{
		return PointAndClickCameras;
	}
};
