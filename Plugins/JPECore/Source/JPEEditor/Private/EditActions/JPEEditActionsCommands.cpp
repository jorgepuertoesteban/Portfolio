// Created by Jorge Puerto. All Rights Reserved.

#include "JPEEditActionsCommands.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 4
#include "Styling/SlateStyle.h"
#include "EditorStyleSet.h"
#elif ENGINE_MAJOR_VERSION == 5
#include "Styling/AppStyle.h"
#endif
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "JPEEditActionsCommands"

void FJPEEditActionsCommands::RegisterCommands()
{
	UI_COMMAND(ExtractStaticMesh, "Create StaticMeshActors from Actors", "Create StaticMeshActors from Actors", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ActorsFromSelection, "Create Actors From Selection", "Create Actors from the selection extracting components and adding them to the newly created Actor", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(RenameSelectedActors, "Rename Selected Actors", "Rename Selected Actors", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CountSelectedActors, "Count Selected Actors", "Count Selected Actors", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ForceMobility, "Force Mobility", "Force Mobility to Selected Actors", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ForceDefaultMaterial, "Force Default Material", "Force Default Material", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(DeleteNullMeshComp, "Delete Components with Null Mesh of Selected Actors", "Delete Components with Null Mesh of Selected Actors", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(HasConstructorComponents, "HasConstructorComponents", "Has Constructor Components", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(MoveActorsToLevelFolder, "MoveActorsToLevelFolder", "Move Actors To Level Folder", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SelectAllSameMesh, "SelectAllSameMesh", "Select all Actors with the same Mesh", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CreateFactory, "Create Factory", "Creates a Factory", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
