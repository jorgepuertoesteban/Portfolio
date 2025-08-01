// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FUICommandList;
class FExtender;
class ISkeletalMeshEditor;

class FJPEEditActions 
{

protected:

	TSharedPtr<FUICommandList> CommandList;
	TSharedPtr<FExtender> MenuExtender;

public:

	void Register();

	void Unregister();

	void CreateButtonInContentBar();

	void ExtendEditMenu();

	static void ExtractStaticMesh();
};
