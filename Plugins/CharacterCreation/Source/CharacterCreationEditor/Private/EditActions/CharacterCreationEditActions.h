// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FUICommandList;
class FExtender;
class ISkeletalMeshEditor;

class FCharacterCreationEditActions 
{

protected:

	TSharedPtr<FUICommandList> CommandList;
	TSharedPtr<FExtender> MenuExtender;

public:

	void Register();

	void Unregister();

	void ExtendEditMenu();
};
