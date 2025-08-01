// Created by Jorge Puerto. All Rights Reserved.

#include "CCSkeletalMeshToolsCommands.h"
#include "Styling/AppStyle.h"
#include "Framework/Commands/Commands.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationModule"

void FCCSkeletalMeshToolsCommands::RegisterCommands()
{
	UI_COMMAND(OpenMeshEditor, "Mesh Editor", "Opens Character Creation Mesh Editor", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenPoseMatcher, "Pose Matcher", "Opens Character Creation Pose Matcher Dialog", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CopyBonesLocation, "Copy Bones Location", "Copy Bones Location from any Skeleton", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(CopyBonesToPreview, "Copy Bones To Peview", "Update Preview", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ResetModifiedBones, "Reset Modified Bones", "Reset Bones", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(DeleteExtraBones, "Delete Extra Bones", "Delete the Extra Bones", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SaveCurrentReferencePose, "Save Current Reference Pose", "Save Current Reference Pose", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(BakeCurrentGeometry, "BakeCurrentGeometry", "BakeCurrentGeometry", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ForceAssignSkeleton, "ForceAssignSkeleton", "ForceAssignSkeleton", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
