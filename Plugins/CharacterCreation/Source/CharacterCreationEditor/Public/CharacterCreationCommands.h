// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CharacterCreationStyle.h"
#include "Tools/InteractiveToolsCommands.h"

class FCharacterCreationCommands : public TCommands<FCharacterCreationCommands>
{
public:

	FCharacterCreationCommands()
		: TCommands<FCharacterCreationCommands>(TEXT("CharacterCreation"), NSLOCTEXT("Contexts", "CharacterCreation", "CharacterCreation Plugin"), NAME_None, FCharacterCreationStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> OpenCharacterCreationEditorAction;

	TSharedPtr<FUICommandInfo> OpenSkeletalDialog;

};