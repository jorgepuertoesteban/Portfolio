//Created by Bionic Ape. All Rights Reserved.


#include "PointAndClick/PointAndClickPawn.h"

#include "ActionType.h"
#include "Components/FocusTracerCursorComponent.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FocusInteractionsLog.h"
#include "FocusInteractionsWorldSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/FocusableComponent.h"
#include "Components/PointAndClickHUDComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PointAndClick/PointAndClickCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/FocusTracerWidget.h"
#include "UI/PointAndClickMenuWidget.h"

const FInputModeGameAndUI APointAndClickPawn::GameModeAndUIInputMode = FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetHideCursorDuringCapture(false);

int32 APointAndClickPawn::DebugPointAndClick = 0;
FAutoConsoleVariableRef ConVarDebugPointAndClickFocus(TEXT("dis.debug-pointandclickfocus"), APointAndClickPawn::DebugPointAndClick, TEXT("Draw debug for Point and Click"), ECVF_Cheat);


// Sets default values
APointAndClickPawn::APointAndClickPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	FocusTracer = CreateDefaultSubobject<UFocusTracerCursorComponent>(TEXT("FocusTracer"));
	FocusTracer->PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
	FocusTracer->PrimaryComponentTick.bStartWithTickEnabled = false;
	FocusTracer->PrimaryComponentTick.bCanEverTick = true;

	ShapeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShapeCollision"));
	ShapeCollision->SetupAttachment(GetRootComponent());
	ShapeCollision->OnComponentBeginOverlap.AddDynamic(this, &APointAndClickPawn::OnComponentBeginOverlap);
	ShapeCollision->PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
	ShapeCollision->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void APointAndClickPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void APointAndClickPawn::BeginPlay()
{
	Super::BeginPlay();

	if (UGameViewportSubsystem* Subsystem = UGameViewportSubsystem::Get(GetWorld()))
	{
		Subsystem->OnWidgetRemoved.AddWeakLambda(this, [this](UWidget*)
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		});
	}
}

void APointAndClickPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SpawnAIControllerAndPawn();
}

void APointAndClickPawn::OnMoveCompleted(FAIRequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		Client_DestroySelectionCircle();
	}
}

void APointAndClickPawn::Destroyed()
{
	if (PointAndClickAIController)
	{
		PointAndClickAIController->Destroy();
	}
	if (AIPawn)
	{
		AIPawn->Destroy();
	}
	if (SelectionCircle)
	{
		SelectionCircle->Destroy();
	}
	if (UFocusTracerComponent* FocusComponent = Cast<UFocusTracerComponent>(UFocusComponent::GetFocusComponent(GetController())))
	{
		FocusComponent->bShouldTrace = bControllerShouldTraceCache;
	}
	Super::Destroyed();
}

void APointAndClickPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsLocallyControlled())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			RemovePlayerInput(PlayerController);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void APointAndClickPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APointAndClickPawn, AIPawn);
}

void APointAndClickPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	FindCamera();

	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}
	PlayerController->CurrentMouseCursor = EMouseCursor::Default;
	UWidgetBlueprintLibrary::SetFocusToGameViewport();

	FocusTracer->PrimaryComponentTick.SetTickFunctionEnable(true);

	if (!FocusTracerWidget)
	{
		FocusTracerWidget = CreateWidget<UFocusTracerWidget>(PlayerController, FocusTracerWidgetClass);
	}
	PlayerController->SetMouseCursorWidget(EMouseCursor::Crosshairs, FocusTracerWidget);
	
	if (UFocusTracerComponent* FocusComponent = Cast<UFocusTracerComponent>(UFocusComponent::GetFocusComponent(GetController())))
	{
		bControllerShouldTraceCache = FocusComponent->bShouldTrace;
		FocusComponent->bShouldTrace = false;
	}

	if (const AHUD* HUD = PlayerController->GetHUD())
	{
		PointAndClickHUDComponent = UPointAndClickHUDComponent::GetPointAndClickHUDComponent(HUD);
	}
}

APlayerController* APointAndClickPawn::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

void APointAndClickPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = GetPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Handles.Add(EnhancedInputComponent->BindAction(PACMouse1Action, ETriggerEvent::Triggered, this, &APointAndClickPawn::Mouse1).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACMouse1DoubleClick, ETriggerEvent::Completed, this, &APointAndClickPawn::Mouse1Double).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACMouse2Action, ETriggerEvent::Triggered, this, &APointAndClickPawn::Mouse2).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACMouse3Action, ETriggerEvent::Triggered, this, &APointAndClickPawn::Mouse3).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum1, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum1).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum2, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum2).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum3, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum3).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum4, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum4).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum5, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum5).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum6, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum6).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum7, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum7).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum8, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum8).GetHandle());
		Handles.Add(EnhancedInputComponent->BindAction(PACKeyboardNum9, ETriggerEvent::Triggered, this, &APointAndClickPawn::KeyboardNum9).GetHandle());
	}
}

void APointAndClickPawn::RemovePlayerInput(const APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		for (const int32 Handle : Handles)
		{
			EnhancedInputComponent->RemoveBindingByHandle(Handle);
		}
		Handles.Empty();
	}
}

void APointAndClickPawn::NotifyControllerChanged()
{
	if (PreviousController)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(PreviousController))
		{
			RemovePlayerInput(PlayerController);
		}
	}
	Super::NotifyControllerChanged(); //IMPORTANT THIS MUST BE CALLED AT THE END!
}

void APointAndClickPawn::SetCameraActor(APointAndClickCamera* NewCamera)
{
	if (APlayerController* PC = GetPlayerController())
	{
		//Only if we are possessed by the player
		CurrentCameraActor = NewCamera;
		PC->SetViewTarget(NewCamera);
	}
}

void APointAndClickPawn::MoveAIPawnToLocation(const FVector& NewLocation)
{
	if (PointAndClickAIController)
	{
		if (UBlackboardComponent* BlackboardComp = PointAndClickAIController->GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsVector(TEXT("Destiny"), NewLocation);
		}
	}
}

void APointAndClickPawn::Server_MoveAIPawnToLocation_Implementation(const FVector& NewLocation)
{
	MoveAIPawnToLocation(NewLocation);
}

void APointAndClickPawn::Client_SpawnSelectionCircleAtLocation_Implementation(const FVector& SpawnLocation, const FVector& HitNormal)
{
	if (IsValid(SelectionCircle))
	{
		SelectionCircle->Destroy();
	}

	if (SelectionCircleClass)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(FRotationMatrix::MakeFromZ(HitNormal).ToQuat());
		SelectionCircle = GetWorld()->SpawnActor<AActor>(SelectionCircleClass, SpawnTransform);
	}
}

void APointAndClickPawn::Client_DestroySelectionCircle_Implementation()
{
	if (IsValid(SelectionCircle))
	{
		SelectionCircle->Destroy();
	}
}

void APointAndClickPawn::Server_SpawnAIControllerAndPawn_Implementation()
{
	SpawnAIControllerAndPawn();
}

void APointAndClickPawn::ScheduleFindCamera()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APointAndClickPawn::FindCamera);
}

void APointAndClickPawn::FindCamera()
{
	CurrentCameraActor = nullptr;
	if (!AIPawn)
	{
		UE_LOG(FocusInteractionsLog, Warning, TEXT("No AIPawn Found"))
		return;
	}
	if (!GetWorld())
	{
		UE_LOG(FocusInteractionsLog, Warning, TEXT("No valid World Found"))
		return;
	}
	const UFocusInteractionsWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UFocusInteractionsWorldSubsystem>();
	for (APointAndClickCamera* CameraActor : WorldSubsystem->GetPointAndClickCameras())
	{
		if (UKismetMathLibrary::IsPointInBoxWithTransform(
				AIPawn->GetActorLocation(),
				CameraActor->InitializationDetectionCollision->GetComponentTransform(),
				CameraActor->InitializationDetectionCollision->GetUnscaledBoxExtent())
		)
		{
			SetCameraActor(CameraActor);
			return;
		}
	}

	//we haven't found a camera, so we schedule looking for one in the next tick
	ScheduleFindCamera();
}

void APointAndClickPawn::OnRep_AIPawn()
{
	FindCamera();
}

void APointAndClickPawn::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APointAndClickCamera* PointAndClickCamera = Cast<APointAndClickCamera>(Other))
	{
		SetCameraActor(PointAndClickCamera);
	}
}

void APointAndClickPawn::SpawnAIControllerAndPawn()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		//this should only be executed with authority role		
		return;
	}

	bool const bIsNewPawnCreated = SpawnAIController();
	bool const bIsNewControllerCreated = SpawnAIPawn();
	if (bIsNewPawnCreated && bIsNewControllerCreated)
	{
		PointAndClickAIController->Possess(AIPawn);
		ShapeCollision->AttachToComponent(AIPawn->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	FindCamera();
}

bool APointAndClickPawn::SpawnAIController()
{
#pragma region Checks
	if (GetLocalRole() < ROLE_Authority)
	{
		return false;
	}
	if (!PointAndClickAIControllerClass)
	{
		return false;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		//TODO: CHECK
		return false;
	}
#pragma endregion

	if (PointAndClickAIController) //Destroy previously created
	{
		PointAndClickAIController->Destroy();
	}
	//New Controller for AI Point and Click Pawn
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags |= RF_Transient;
	PointAndClickAIController = World->SpawnActor<AAIController>(PointAndClickAIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
	PointAndClickAIController->ReceiveMoveCompleted.AddDynamic(this, &APointAndClickPawn::OnMoveCompleted);
	return PointAndClickAIController != nullptr;
}

bool APointAndClickPawn::SpawnAIPawn()
{
#pragma region Checks
	if (GetLocalRole() < ROLE_Authority)
	{
		return false;
	}
	if (!AIPawnClass)
	{
		return false;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		//TODO: CHECK
		return false;
	}
#pragma endregion
	if (AIPawn) //Destroy previously created
	{
		AIPawn->Destroy();
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags |= RF_Transient;
	AIPawn = World->SpawnActor<APawn>(AIPawnClass, GetActorLocation(), GetActorRotation(), SpawnInfo);

	return AIPawn != nullptr;
}

void APointAndClickPawn::WalkCloserPerformAction(int32 ActionIndex)
{
	//Get player character distance to interactable actor
	FVector FocusableLocation = LastFarFocusPerformAction.FocusedActor->GetActorLocation();
	FVector PawnLocation = LastFarFocusPerformAction.ActionPawn->GetActorLocation(); //This location is not updated at this point and conserves the value seted when the pawn is spawned (or so it seems)

	float DistanceFromPawn = FVector::Dist(FocusableLocation, PawnLocation);

	//Check if the character is close enough
	float ActionMaxDistance = LastFarFocusPerformAction.ActionType->GetMaxDistance();
	if (DistanceFromPawn > ActionMaxDistance)
	{
		//Move the character to the closer free space on the map
		FVector PawnToFocusable = FVector(FocusableLocation.X, FocusableLocation.Y, PawnLocation.Z) - PawnLocation; //Set the same altitude on both ends 
		PawnToFocusable.Normalize();
		float ClosestDistanceToInteractable = DistanceFromPawn - ActionMaxDistance;


		FVector NewPawnLocation = PawnLocation + (PawnToFocusable * ClosestDistanceToInteractable);

		//Find the floor on the closest point to the interactable
		FVector StartTrace = NewPawnLocation;
		FVector EndTrace = NewPawnLocation + (FVector(0.0f, 0.0f, -1.0f) * DistanceFromPawn);

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, TraceChannel, CollisionParams))
		{
			//Interact with it
			Server_MoveCharacterWithNotify(Hit.Location, Hit.Location, Hit.Normal);
		}
	}
	else
	{
		FocusTracer->PerformAction(ActionIndex);
	}
}

void APointAndClickPawn::Server_MoveCharacterWithNotify_Implementation(FVector SelectionCircleLocation, FVector CharacterLocation, FVector HitNormal)
{
	MoveCharacter(SelectionCircleLocation, CharacterLocation, HitNormal);
	if (PointAndClickAIController)
	{
		PointAndClickAIController->ReceiveMoveCompleted.AddDynamic(this, &APointAndClickPawn::CharacterMovementCompleted);
	}
}

void APointAndClickPawn::Server_CharacterMovementCompleted_Implementation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	CharacterMovementCompleted(RequestID, Result);
}

void APointAndClickPawn::CharacterMovementCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Server_ClearCharacterMovementCompletedEvent();
	Client_PerformAction(RequestID, Result);
}

void APointAndClickPawn::Client_PerformAction_Implementation(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	FocusTracer->PerformActionWithFocusPerformAction(LastFarFocusPerformAction);
}

void APointAndClickPawn::Server_ClearCharacterMovementCompletedEvent_Implementation()
{
	ClearCharacterMovementCompletedEvent();
}

void APointAndClickPawn::ClearCharacterMovementCompletedEvent()
{
	if (PointAndClickAIController)
	{
		PointAndClickAIController->ReceiveMoveCompleted.RemoveDynamic(this, &APointAndClickPawn::CharacterMovementCompleted);
	}
}

void APointAndClickPawn::MoveCharacterToMouseLocation()
{
	APlayerController* MyPlayerController = GetPlayerController();
	if (MyPlayerController && GetWorld())
	{
		FVector2D MousePosition;
		MyPlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		FVector StartTrace;
		FVector RotVector;
		UGameplayStatics::DeprojectScreenToWorld(MyPlayerController, MousePosition, StartTrace, RotVector);

		const FVector EndTrace = StartTrace + (RotVector * TraceDistance);

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (DebugPointAndClick > 0)
		{
			static FName const TraceTag("CollectorTraceTag");
			GetWorld()->DebugDrawTraceTag = TraceTag;
			CollisionParams.TraceTag = TraceTag;
		}
#endif
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, TraceChannel, CollisionParams))
		{
			MoveCharacter(Hit.Location, Hit.Location, Hit.Normal);
		}
	}
}

void APointAndClickPawn::MoveCharacter(const FVector& SelectionCircleLocation, const FVector& CharacterLocation, const FVector& HitNormal)
{
	Client_SpawnSelectionCircleAtLocation(SelectionCircleLocation, HitNormal);
	Server_MoveAIPawnToLocation(CharacterLocation);
}

UFocusComponent* APointAndClickPawn::GetFocusComponent_Implementation() const
{
	return FocusTracer;
}

UFocusableComponent* APointAndClickPawn::GetFocusableComponent_Implementation()
{
	return UFocusableComponent::GetFocusableComponent(AIPawn);
}

void APointAndClickPawn::Mouse1Double()
{
}

void APointAndClickPawn::Mouse1()
{
	//Get distance to player pawn and test if it is close enough
	LastFarFocusPerformAction.Reset();
	FocusTracer->GetPerformActionWithIndex(0, LastFarFocusPerformAction);

	Server_ClearCharacterMovementCompletedEvent();

	if (LastFarFocusPerformAction.ActionPawn && LastFarFocusPerformAction.Focusable)
	{
		WalkCloserPerformAction(0);
	}
}

void APointAndClickPawn::Mouse2()
{
	LastFarFocusPerformAction.Reset();
	FocusTracer->GetPerformActionWithIndex(1, LastFarFocusPerformAction);

	Server_ClearCharacterMovementCompletedEvent();

	if (LastFarFocusPerformAction.ActionPawn && LastFarFocusPerformAction.Focusable)
	{
		WalkCloserPerformAction(1);
	}
	else
	{
		//NO interactable clicked
		MoveCharacterToMouseLocation();
	}
}

void APointAndClickPawn::Mouse3()
{
	LastFarFocusPerformAction.Reset();
	FocusTracer->GetPerformActionWithIndex(2, LastFarFocusPerformAction);

	Server_ClearCharacterMovementCompletedEvent();

	if (LastFarFocusPerformAction.ActionPawn && LastFarFocusPerformAction.Focusable)
	{
		WalkCloserPerformAction(2);
	}
}

void APointAndClickPawn::KeyboardNum1()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(0);
	}
}

void APointAndClickPawn::KeyboardNum2()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(1);
	}
}

void APointAndClickPawn::KeyboardNum3()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(2);
	}
}

void APointAndClickPawn::KeyboardNum4()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(3);
	}
}

void APointAndClickPawn::KeyboardNum5()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(4);
	}
}

void APointAndClickPawn::KeyboardNum6()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(5);
	}
}

void APointAndClickPawn::KeyboardNum7()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(6);
	}
}

void APointAndClickPawn::KeyboardNum8()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(7);
	}
}

void APointAndClickPawn::KeyboardNum9()
{
	if (PointAndClickHUDComponent)
	{
		PointAndClickHUDComponent->SelectAction(8);
	}
}
