// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/InputModePawnInterface.h"
#include "InputModeHelper.generated.h"

/**
 * 
 */
UCLASS()
class JPEUI_API UInputModeHelper : public UObject
{
	GENERATED_BODY()

public:
	static const FInputModeGameAndUI InputModeGameAndUI;
	static const FInputModeGameOnly InputModeGameOnly;
	static const FInputModeUIOnly InputModeUIOnly;

	static inline void SetInputMode(APlayerController* PlayerController, EInputMode InputMode)
	{
		if (!PlayerController)
		{
			return;
		}

		switch (InputMode)
		{
		case EInputMode::GameAndUI:
			SetInputModeGameAndUI(PlayerController);
			break;
		case EInputMode::GameOnly:
			SetInputModeGameOnly(PlayerController);
			break;
		case EInputMode::UIOnly:
			SetInputUIOnly(PlayerController);
			break;
		}
	}

	static inline void SetInputModeGameAndUI(APlayerController* PlayerController)
	{
		if (PlayerController && PlayerController->IsLocalPlayerController())
		{
			PlayerController->SetInputMode(InputModeGameAndUI);
			PlayerController->bShowMouseCursor = true;
		}
	}

	static inline void SetInputUIOnly(APlayerController* PlayerController)
	{
		if (PlayerController && PlayerController->IsLocalPlayerController())
		{
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->bShowMouseCursor = true;
		}
	}

	static inline void SetInputModeGameOnly(APlayerController* PlayerController)
	{
		if (PlayerController && PlayerController->IsLocalPlayerController())
		{
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->bShowMouseCursor = false;
		}
	}

	static inline void SetInputModeFromPawn(APlayerController* PlayerController, APawn* Pawn)
	{
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetInputModeGameOnly: PlayerController is null"));
			return;
		}

		if (!Pawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetInputModeGameOnly: Pawn is null"));
			return;
		}

		if (Pawn->Implements<UInputModePawnInterface>())
		{
			UInputModeHelper::SetInputMode(PlayerController, IInputModePawnInterface::Execute_GetInputMode(Pawn));
		}
	}
};
