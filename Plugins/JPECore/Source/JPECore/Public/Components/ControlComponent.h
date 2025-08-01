// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/JPEHUDInterface.h"
#include "GameFramework/HUD.h"
#include "ControlComponent.generated.h"

class APlayerController;
class APawn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JPECORE_API UControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	APlayerController* GetController() const;

	UFUNCTION(BlueprintCallable)
	APawn* GetControlledPawn() const;
	
	UFUNCTION(BlueprintCallable)
	AHUD* GetHUD() const;

	TScriptInterface<IJPEHUDInterface> GetJPEHUD() const;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_Notify(bool bIsSuccessful, const FString& Message);

};
