// Created by Jorge Puerto. All Rights Reserved.

#include "JPEMainMenuSystem/JPEMainMenuSystemCommands.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "JPEMainMenuSystemCommands"

void FJPEMainMenuSystemCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "JPE Editor", "Open JPE Plugin window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE