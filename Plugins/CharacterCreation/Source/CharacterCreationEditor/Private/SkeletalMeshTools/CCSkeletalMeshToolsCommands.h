// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CharacterCreationStyle.h"
#include "Tools/InteractiveToolsCommands.h"

class FCCSkeletalMeshToolsCommands : public TCommands<FCCSkeletalMeshToolsCommands>
{
public:

	FCCSkeletalMeshToolsCommands()
		: TCommands<FCCSkeletalMeshToolsCommands>(TEXT("CharacterCreation"), NSLOCTEXT("Contexts", "CharacterCreation", "CharacterCreation Plugin"), NAME_None, FCharacterCreationStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> OpenMeshEditor;
	TSharedPtr<FUICommandInfo> OpenPoseMatcher;
	TSharedPtr<FUICommandInfo> CopyBonesLocation;
	TSharedPtr<FUICommandInfo> CopyBonesToPreview;
	TSharedPtr<FUICommandInfo> ResetModifiedBones;
	TSharedPtr<FUICommandInfo> DeleteExtraBones;
	TSharedPtr<FUICommandInfo> SaveCurrentReferencePose;
	TSharedPtr<FUICommandInfo> BakeCurrentGeometry;
	TSharedPtr<FUICommandInfo> ForceAssignSkeleton;

};