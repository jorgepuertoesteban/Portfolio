// Created by Jorge Puerto. All Rights Reserved.

#include "ModuleGenerationCommands.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "FModuleGenerationModule"

void FModuleGenerationCommands::RegisterCommands()
{
    UI_COMMAND(NewModule, "New C++ module...", "Creates a new game module in this project", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE