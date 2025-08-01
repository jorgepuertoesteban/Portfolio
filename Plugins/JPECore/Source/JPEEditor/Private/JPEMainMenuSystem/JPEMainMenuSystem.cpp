// Created by Jorge Puerto. All Rights Reserved.

#include "JPEMainMenuSystem/JPEMainMenuSystem.h"
#include "JPEMainMenuSystem/JPEMainMenuSystemCommands.h"
#include "JPEMainMenuSystem/SJPEEditorMenu.h"

#include "JPEEditorStyle.h"

#include "Framework/Docking/TabManager.h"
#include "Framework/Commands/UICommandList.h"

#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuEntry.h"

#include "Framework/Commands/UIAction.h"
#include "WorkspaceMenuStructureModule.h"
#include "WorkspaceMenuStructure.h"


#define LOCTEXT_NAMESPACE "MainMenuSystem"

namespace JPEMainMenu
{
	const FName JPEMainMenuSystemWindowID = FName(TEXT("JPEMainMenuSystem"));

	TSharedRef<class SDockTab> SpawnNomadTab(const FSpawnTabArgs& Args)
	{
		return SNew(SDockTab)
			.TabRole(NomadTab)
			[
				SNew(SJPEEditorMenu)
			];
	}
}

void FJPEMainMenuSystem::Register()
{
}

void FJPEMainMenuSystem::Unregister()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(JPEMainMenu::JPEMainMenuSystemWindowID);
}

void FJPEMainMenuSystem::Launch()
{
	FGlobalTabmanager::Get()->TryInvokeTab(JPEMainMenu::JPEMainMenuSystemWindowID);
}

#undef LOCTEXT_NAMESPACE
