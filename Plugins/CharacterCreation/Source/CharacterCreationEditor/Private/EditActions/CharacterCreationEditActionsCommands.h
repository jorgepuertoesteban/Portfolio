// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CharacterCreationStyle.h"
#include "Tools/InteractiveToolsCommands.h"

class FCharacterCreationEditActionsCommands : public TCommands<FCharacterCreationEditActionsCommands>
{
public:

	FCharacterCreationEditActionsCommands()
		: TCommands<FCharacterCreationEditActionsCommands>(TEXT("CharacterCreationEditActions"), NSLOCTEXT("Contexts", "CharacterCreationEditActions", "CharacterCreationEditActions"), NAME_None, FCharacterCreationStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> AddSkeletalsToCharacterCreationGrooms;
	TSharedPtr<FUICommandInfo> GenerateCharacterCreatorGroom;


};