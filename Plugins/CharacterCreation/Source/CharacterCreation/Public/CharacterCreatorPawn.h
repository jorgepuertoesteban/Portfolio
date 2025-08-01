// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterCreatorPawn.generated.h"

class AActor;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UCharacterCreatorControlComponent;
class UCharacterCreatorComponent;
class APlayerCameraManager;

UCLASS()
class CHARACTERCREATION_API ACharacterCreatorPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="")
	bool bSyncWithControlComponent = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCharacterCreatorComponent* CharacterCreatorComponent;

	UPROPERTY(Transient)
	TMap<FName, AActor*> CamerasMap;

	UPROPERTY(Transient)
	UCharacterCreatorControlComponent* ControlComponent;

	UPROPERTY(Transient)
	APlayerCameraManager* PlayerCameraManager;

public:
	ACharacterCreatorPawn();

public:
	virtual void PawnClientRestart() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category="Character Creation")
	void ShowCamera(const FName& CameraTag);
};
