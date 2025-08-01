// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "JPEEditorStyle.h"
/**
 *
 */
class FJPEEditorCommands : public TCommands<FJPEEditorCommands>
{
public:
	FJPEEditorCommands() :
		TCommands<FJPEEditorCommands>(
			TEXT("JPEEditor"), //Context name for fast lookup
			NSLOCTEXT("Contexts", "JPEEditor", "JPE Editor"), //Localized context name for displaying
			NAME_None, // Parent
			FJPEEditorStyle::GetAppStyleSetName()// Icon Style Set
			)
	{
	}

	virtual void RegisterCommands() override;

	//TSharedPtr<FUICommandInfo> OpenModuleGeneratorTool;

	//TSharedPtr<FUICommandInfo> NewModule;

	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
