// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "JPEEditorStyle.h"
#include "Tools/InteractiveToolsCommands.h"

class FJPEEditActionsCommands : public TCommands<FJPEEditActionsCommands>
{
public:

	FJPEEditActionsCommands()
		: TCommands<FJPEEditActionsCommands>(TEXT("JPEEditActions"), NSLOCTEXT("Contexts", "JPEEditActions", "JPEEditActions"), NAME_None, FJPEEditorStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> ExtractStaticMesh;
	TSharedPtr<FUICommandInfo> ActorsFromSelection;
	TSharedPtr<FUICommandInfo> RenameSelectedActors;
	TSharedPtr<FUICommandInfo> CountSelectedActors;
	TSharedPtr<FUICommandInfo> ForceMobility;
	TSharedPtr<FUICommandInfo> ForceDefaultMaterial;
	TSharedPtr<FUICommandInfo> DeleteNullMeshComp;
	TSharedPtr<FUICommandInfo> HasConstructorComponents;
	TSharedPtr<FUICommandInfo> MoveActorsToLevelFolder;
	TSharedPtr<FUICommandInfo> SelectAllSameMesh;	
	TSharedPtr<FUICommandInfo> CreateFactory;
};