// Created by Jorge Puerto. All Rights Reserved.

#include "JPEEditorCommands.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "JPEEditorCommands"

void FJPEEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "JPE Editor", "Open JPE Plugin window", EUserInterfaceActionType::Button, FInputChord());
	//UI_COMMAND(OpenModuleGeneratorTool, "Module Generator", "Bring up ModuleGenerator window", EUserInterfaceActionType::Button, FInputChord());
	//UI_COMMAND(NewModule, "New C++ module...", "Creates a new game module in this project", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE