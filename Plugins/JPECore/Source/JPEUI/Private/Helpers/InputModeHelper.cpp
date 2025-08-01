// Created by Jorge Puerto. All Rights Reserved.


#include "Helpers/InputModeHelper.h"

const FInputModeGameAndUI UInputModeHelper::InputModeGameAndUI = FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetHideCursorDuringCapture(false);
const FInputModeGameOnly UInputModeHelper::InputModeGameOnly = FInputModeGameOnly();
const FInputModeUIOnly UInputModeHelper::InputModeUIOnly = FInputModeUIOnly();
