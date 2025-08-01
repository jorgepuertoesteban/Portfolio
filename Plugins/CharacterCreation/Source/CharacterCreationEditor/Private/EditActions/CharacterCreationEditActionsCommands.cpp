// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreationEditActionsCommands.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 4
#include "Styling/SlateStyle.h"
#include "EditorStyleSet.h"
#elif ENGINE_MAJOR_VERSION == 5
#include "Styling/AppStyle.h"
#endif
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "CharacterCreationEditActionsCommands"

void FCharacterCreationEditActionsCommands::RegisterCommands()
{
	UI_COMMAND(AddSkeletalsToCharacterCreationGrooms, "Add Skeletals To CharacterCreationGrooms", "Add Skeletals To CharacterCreationGrooms", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(GenerateCharacterCreatorGroom, "Generate CharacterCreator Groom", "Generates CharacterCreator Groom", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
