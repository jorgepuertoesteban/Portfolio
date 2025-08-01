// Created by Jorge Puerto. All Rights Reserved.



#include "EditActions/CharacterCreationEditActions.h"
#include "EditActions/CharacterCreationEditActionsCommands.h"
#include "EditActions/AddSkeletalsToCCGrooms/CCEditAction_AddSkeletalsToCCGrooms.h"
#include "EditActions/GenerateCharacterCreatorGroom/CharacterCreationEditAction_GenerateCharacterCreatorGroom.h"
#include "LevelEditor.h"

#include "ToolMenu.h"
#include "ToolMenus.h"
#include "CharacterCreationStyle.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationEditActions"

void FCharacterCreationEditActions::Register()
{
	FCharacterCreationEditActionsCommands::Register();
	
	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		FCharacterCreationEditActionsCommands::Get().AddSkeletalsToCharacterCreationGrooms,
		FExecuteAction::CreateStatic(&FCCEditAction_AddSkeletalsToCCGrooms::ExecuteAction),
		FCanExecuteAction()
	);

	CommandList->MapAction(
		FCharacterCreationEditActionsCommands::Get().GenerateCharacterCreatorGroom,
		FExecuteAction::CreateStatic(&FCharacterCreationEditAction_GenerateCharacterCreatorGroom::ExecuteAction),
		FCanExecuteAction()
	);

	ExtendEditMenu();
}

void FCharacterCreationEditActions::ExtendEditMenu()
{
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuExtension(
		"EditMain",
		EExtensionHook::After,
		CommandList.ToSharedRef(),
		FMenuExtensionDelegate::CreateLambda(
			[this](FMenuBuilder& MenuBuilder)
			{
				FSlateIcon MenuIcon = FSlateIcon(FCharacterCreationStyle::GetAppStyleSetName(), "CharacterCreation.Image");
				MenuBuilder.AddSubMenu(
					LOCTEXT("CharacterCreation", "CharacterCreation"),
					LOCTEXT("CharacterCreation", "CharacterCreation"),
					FNewMenuDelegate::CreateLambda(
						[this](class FMenuBuilder& MenuBuilder)
						{
							MenuBuilder.AddMenuEntry(FCharacterCreationEditActionsCommands::Get().AddSkeletalsToCharacterCreationGrooms);
							MenuBuilder.AddMenuEntry(FCharacterCreationEditActionsCommands::Get().GenerateCharacterCreatorGroom);
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

void FCharacterCreationEditActions::Unregister()
{
	FCharacterCreationEditActionsCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE