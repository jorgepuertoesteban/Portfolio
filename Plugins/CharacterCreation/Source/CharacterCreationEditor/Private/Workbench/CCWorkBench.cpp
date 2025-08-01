// Created by Jorge Puerto. All Rights Reserved.

#include "Workbench/CCWorkBench.h"
//#include "Workbench/CCWorkbenchCommands.h"
#include "Workbench/SCCWorkbench.h"

#include "CharacterCreationStyle.h"

#include "Framework/Docking/TabManager.h"
#include "Framework/Commands/UICommandList.h"

#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuEntry.h"

#include "Framework/Commands/UIAction.h"
#include "WorkspaceMenuStructureModule.h"
#include "WorkspaceMenuStructure.h"


#define LOCTEXT_NAMESPACE "MainMenuSystem"

namespace
{
	const FName CCWorkbenchWindowID = FName(TEXT("CCWorkbench"));

	TSharedRef<class SDockTab> SpawnNomadTab(const FSpawnTabArgs& Args)
	{
		return SNew(SDockTab)
			.TabRole(NomadTab)
			[
				SNew(SCCWorkbench)
			];
	}
}

void FCCWorkbench::Register()
{
}

void FCCWorkbench::Unregister()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CCWorkbenchWindowID);
}

void FCCWorkbench::Launch()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CCWorkbenchWindowID);
}

#undef LOCTEXT_NAMESPACE
