// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FocusInteractionsTypes.h"
#include "Interfaces/FocusableComponentOwner.h"
#include "Interfaces/FocusComponentOwner.h"
#include "Navigation/PathFollowingComponent.h"
#include "PointAndClickPawn.generated.h"

class UPointAndClickHUDComponent;
class UInputMappingContext;
class UInputAction;
class AAIController;
class UFocusTracerCursorComponent;
class APointAndClickCamera;
class UBoxComponent;
class UFocusTracerWidget;

UCLASS()
class FOCUSINTERACTIONS_API APointAndClickPawn :
	public APawn,
	public IFocusComponentOwner,
	public IFocusableComponentOwner
{
	GENERATED_BODY()

public:
	static int32 DebugPointAndClick;
	static const FInputModeGameAndUI GameModeAndUIInputMode;

protected:
	UPROPERTY(Transient)
	bool bControllerShouldTraceCache;

	UPROPERTY(Transient)
	UPointAndClickHUDComponent* PointAndClickHUDComponent;

	UPROPERTY(EditAnywhere, Category = "PointAndClick")
	TSubclassOf<APawn> AIPawnClass;
	UPROPERTY(ReplicatedUsing="OnRep_AIPawn", Transient, BlueprintReadWrite, Category = "PointAndClick")
	APawn* AIPawn;

	UPROPERTY(EditAnywhere, Category = "PointAndClick")
	TSubclassOf<AAIController> PointAndClickAIControllerClass;
	UPROPERTY(Transient, Category = "PointAndClick", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AAIController* PointAndClickAIController;

	UPROPERTY(EditAnywhere, Category = "PointAndClick")
	TSubclassOf<AActor> SelectionCircleClass;
	UPROPERTY(Transient, Category = "PointAndClick", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* SelectionCircle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointAndClick")
	float TraceDistance = 10000.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointAndClick")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "PointAndClick")
	UFocusTracerCursorComponent* FocusTracer;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "PointAndClick")
	APointAndClickCamera* CurrentCameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "PointAndClick")
	UBoxComponent* ShapeCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Focus")
	TSubclassOf<UFocusTracerWidget> FocusTracerWidgetClass;
	UPROPERTY(Transient)
	UFocusTracerWidget* FocusTracerWidget;

	UPROPERTY(Transient)
	FFocusPerformAction LastFarFocusPerformAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Mouse1 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACMouse1Action;

	/** Mouse1 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACMouse1DoubleClick;

	/** Mouse2 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACMouse2Action;

	/** Mouse3 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACMouse3Action;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum1;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum2;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum3;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum4;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum5;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum6;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum7;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum8;

	/** Keyboard 1 Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PACKeyboardNum9;

	UPROPERTY(Transient)
	TArray<int32> Handles;

public:
	APointAndClickPawn(const FObjectInitializer& ObjectInitializer);

	virtual void PreInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	const AActor* GetAIPawn() const { return AIPawn; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:
	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PawnClientRestart() override;

	APlayerController* GetPlayerController() const;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RemovePlayerInput(const APlayerController* PlayerController);

#pragma region IFocusComponentOwner
	virtual UFocusComponent* GetFocusComponent_Implementation() const override;
#pragma endregion

#pragma region IFocusableComponentOwner

public:
	virtual UFocusableComponent* GetFocusableComponent_Implementation() override;
#pragma endregion

	APointAndClickCamera* GetCurrentCameraActor() const { return CurrentCameraActor; }

	UFUNCTION(BlueprintCallable, Category = "PointAndClick")
	void SetCameraActor(APointAndClickCamera* NewCamera);

	UFUNCTION()
	void MoveCharacterToMouseLocation();

	UFUNCTION()
	void MoveCharacter(const FVector& SelectionCircleLocation, const FVector& CharacterLocation, const FVector& HitNormal);

	UFUNCTION()
	virtual void CharacterMovementCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void ClearCharacterMovementCompletedEvent();

	UFUNCTION(Server, Reliable)
	void Server_ClearCharacterMovementCompletedEvent();

	UFUNCTION(Server, Reliable)
	void Server_CharacterMovementCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION(Client, Reliable)
	void Client_PerformAction(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION(Server, Reliable)
	void Server_MoveCharacterWithNotify(FVector SelectionCircleLocation, FVector CharacterLocation, FVector HitNormal);

	UFUNCTION()
	void WalkCloserPerformAction(int32 ActionIndex);

	UFUNCTION()
	virtual void FindCamera();

	virtual void NotifyControllerChanged() override;

protected:
	virtual void Mouse1();
	virtual void Mouse1Double();
	virtual void Mouse2();
	virtual void Mouse3();

	virtual void KeyboardNum1();
	virtual void KeyboardNum2();
	virtual void KeyboardNum3();
	virtual void KeyboardNum4();
	virtual void KeyboardNum5();
	virtual void KeyboardNum6();
	virtual void KeyboardNum7();
	virtual void KeyboardNum8();
	virtual void KeyboardNum9();

	UFUNCTION(BlueprintCallable)
	void MoveAIPawnToLocation(const FVector& NewLocation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_MoveAIPawnToLocation(const FVector& NewLocation);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_SpawnSelectionCircleAtLocation(const FVector& SpawnLocation, const FVector& HitNormal);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_DestroySelectionCircle();

	UFUNCTION(BlueprintCallable)
	void SpawnAIControllerAndPawn();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnAIControllerAndPawn();

	UFUNCTION(BlueprintCallable)
	virtual bool SpawnAIController();
	UFUNCTION(BlueprintCallable)
	virtual bool SpawnAIPawn();

	void ScheduleFindCamera();

	UFUNCTION()
	void OnRep_AIPawn();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
