// Created by Jorge Puerto. All Rights Reserved.



#include "EditActions/JPEEditActions.h"
#include "EditActions/JPEEditActionsCommands.h"

#include "EditActions/JPEEditAction_MeshFromBlueprint.h"
#include "EditActions/JPEEditAction_RenameSelectedActors.h"


#include "LevelEditor.h"

#include "ToolMenu.h"
#include "ToolMenus.h"
#include "JPEEditAction_ActorsFromSelection.h"
#include "JPEEditAction_CountSelectedActors.h"
#include "JPEEditAction_ForceMobility.h"
#include "JPEEditAction_HasConstructorComponents.h"
#include "JPEEditAction_MoveActorsToLevelFolder.h"
#include "JPEEditorStyle.h"
#include "JPEEditAction_ForceDefaultMaterial.h"
#include "JPEEditAction_DeleteNullMeshComp.h"
#include "JPEEditAction_SelectAllSameMesh.h"
#include "CreateFactory/JPEEditAction_CreateFactory.h"

#define LOCTEXT_NAMESPACE "FJPEEditActions"

void FJPEEditActions::Register()
{
	FJPEEditActionsCommands::Register();
	
	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		FJPEEditActionsCommands::Get().ExtractStaticMesh,
		FExecuteAction::CreateStatic(&FJPEEditAction_MeshFromBlueprint::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().ActorsFromSelection,
		FExecuteAction::CreateStatic(&FJPEEditAction_ActorsFromSelection::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().RenameSelectedActors,
		FExecuteAction::CreateStatic(&FJPEEditAction_RenameSelectedActors::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().CountSelectedActors,
		FExecuteAction::CreateStatic(&FJPEEditAction_CountSelectedActors::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().ForceMobility,
		FExecuteAction::CreateStatic(&FJPEEditAction_ForceMobility::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().DeleteNullMeshComp,
		FExecuteAction::CreateStatic(&FJPEEditAction_DeleteNullMeshComp::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().ForceDefaultMaterial,
		FExecuteAction::CreateStatic(&FJPEEditAction_ForceDefaultMaterial::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().HasConstructorComponents,
		FExecuteAction::CreateStatic(&FJPEEditAction_HasConstructorComponents::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().MoveActorsToLevelFolder,
		FExecuteAction::CreateStatic(&FJPEEditAction_MoveActorsToLevelFolder::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().SelectAllSameMesh,
		FExecuteAction::CreateStatic(&FJPEEditAction_SelectAllSameMesh::ExecuteAction),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FJPEEditActionsCommands::Get().CreateFactory,
		FExecuteAction::CreateStatic(&FJPEEditAction_CreateFactory::ExecuteAction),
		FCanExecuteAction()
	);

	//CreateButtonInContentBar();
	ExtendEditMenu();
}

void FJPEEditActions::ExtendEditMenu()
{
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuExtension(
		"EditMain",
		EExtensionHook::After,
		CommandList.ToSharedRef(),
		FMenuExtensionDelegate::CreateLambda(
			[this](FMenuBuilder& MenuBuilder)
			{
				FSlateIcon MenuIcon = FSlateIcon(FJPEEditorStyle::GetAppStyleSetName(), "JPEEditor.Image");
				MenuBuilder.AddSubMenu(
					LOCTEXT("JPEEditActions", "JPEEditActions"),
					LOCTEXT("JPEEditActions", "JPEEditActions"),
					FNewMenuDelegate::CreateLambda(
						[this](class FMenuBuilder& MenuBuilder)
						{
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().ExtractStaticMesh);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().RenameSelectedActors);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().ActorsFromSelection);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().CountSelectedActors);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().ForceMobility);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().ForceDefaultMaterial);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().DeleteNullMeshComp);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().HasConstructorComponents);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().MoveActorsToLevelFolder);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().SelectAllSameMesh);
							MenuBuilder.AddMenuEntry(FJPEEditActionsCommands::Get().CreateFactory);
						}
					),
					false,
					MenuIcon
				);
			}
		)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

}

void FJPEEditActions::CreateButtonInContentBar()
{
	UToolMenu* AssetsToolBar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.AssetsToolBar");
	if (AssetsToolBar)
	{
		FToolMenuEntry ToolMenuEntry = FToolMenuEntry::InitToolBarButton(
			"JPEEditActions",
			FUIAction(
				FExecuteAction::CreateLambda(
					[]()
					{
						
					}
				)
			),
			LOCTEXT("JPEEditActions_Friendly", "JPEEditActions"),
			LOCTEXT("JPEEditActions_Tooltip", "JPEEditActions"),
			FSlateIcon(FJPEEditorStyle::GetAppStyleSetName(), TEXT("JPEEditor.Image")));
		//ToolMenuEntry.StyleNameOverride = "CalloutToolbar";

		FToolMenuSection& Section = AssetsToolBar->AddSection("Content");
		Section.AddEntry(ToolMenuEntry);
	}
}

void FJPEEditActions::Unregister()
{
	FJPEEditActionsCommands::Unregister();
}

void FJPEEditActions::ExtractStaticMesh()
{

}
#undef LOCTEXT_NAMESPACE