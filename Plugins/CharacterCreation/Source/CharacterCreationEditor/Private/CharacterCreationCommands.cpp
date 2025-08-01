// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreationCommands.h"
#include "Styling/AppStyle.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationModule"

void FCharacterCreationCommands::RegisterCommands()
{
	UI_COMMAND(OpenCharacterCreationEditorAction, "Character Creation Editor", "Opens Character Creation Editor", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenSkeletalDialog, "Character Creation Tools", "Opens Character Creation Tools Dialog", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
