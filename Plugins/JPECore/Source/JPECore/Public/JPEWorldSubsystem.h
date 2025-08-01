// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "JPEWorldSubsystem.generated.h"

class AActor;
class APlayerController;

/**
 *
 */
UCLASS(Config = JPE)
class JPECORE_API UJPEWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	TMap<FString, AActor*> TransitionCameraActors;

	UPROPERTY(Transient)
	TMap<FString, AActor*> ActorsByKey;

	static UJPEWorldSubsystem* Instance;

	/**
	 * The Default value is disabled, please configure it accordingly
	 */
	UPROPERTY(Config)
	bool bShouldCreateSubsystem = false;

public:

	template <typename TActorClass>
	TActorClass* GetActorByKey(const FString& Key) const
	{
		if (ActorsByKey.Contains(Key))
		{
			return Cast<TActorClass>(ActorsByKey[Key]);
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	static UJPEWorldSubsystem* GetJPEWorldSubsystem() { return Instance; }

	static UJPEWorldSubsystem* GetJPEWorldSubsystem(AActor* Actor);


	// USubsystem implementation Begin
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	UFUNCTION(BlueprintCallable)
	AActor* GetRegisteredCameraActors(const FString& TransitionCameraString);

	UFUNCTION(BlueprintCallable)
	void RegisterTransitionCamera(const FString& TransitionCameraString, AActor* TransitionCamera);
	
	UFUNCTION(BlueprintCallable)
	void RegisterActor(const FString& Key, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void SetViewTargetForTransitionCameraByString(const FString& TransitionCameraString, APlayerController* PlayerController);

};
