// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "JPEEditorStyle.h"
#include "Framework/Commands/UICommandList.h"
/**
 *
 */
class FJPEMainMenuSystemCommands : public TCommands<FJPEMainMenuSystemCommands>
{
public:
	FJPEMainMenuSystemCommands()
		: TCommands<FJPEMainMenuSystemCommands>(
			TEXT("JPEEditor"), //Context name for fast lookup
			NSLOCTEXT("Contexts", "JPEEditor", "JPE Editor"), //Localized context name for displaying
			NAME_None, // Parent
			FJPEEditorStyle::GetAppStyleSetName()// Icon Style Set
			)//
	{
	}

	//TCommand interface
	virtual void RegisterCommands() override;
	//End TCommand interface

	TSharedPtr<FUICommandInfo> OpenModuleGeneratorTool;

	TSharedPtr<FUICommandInfo> NewModule;

	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
