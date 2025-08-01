// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_MoveActorsToLevelFolder.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"
#include "Misc/MessageDialog.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"

#include "EditActions/JPEEditActionsLib.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_MoveActorsToLevelFolder"


void FJPEEditAction_MoveActorsToLevelFolder::ExecuteAction()
{

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		UE_LOG(LogTemp, Log, TEXT("Not valid World"));
		return;
	}

	GEditor->BeginTransaction(LOCTEXT("JPEEditAction_MoveActorsToLevelFolder", "Move actors to level folder"));

	EAppReturnType::Type WindowReturn = FMessageDialog::Open(EAppMsgType::YesNoYesAll, LOCTEXT("IterateAllActors", "Move Selected actors to level folder? (Yes all to do all actors)?"));

	if (WindowReturn == EAppReturnType::Yes)
	{
		USelection* SelectedActors = GEditor->GetSelectedActors();
		for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
		{
			if (AActor* Actor = Cast<AActor>(*Iter))
			{
				FJPEEditActionsLib::CreateFolderPath(Actor, Actor->GetClass(), true, Actor);
			}
		}
	}
	else if (WindowReturn == EAppReturnType::YesAll)
	{
		for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
		{
			if (AActor* Actor = *It)
			{
				FJPEEditActionsLib::CreateFolderPath(Actor, Actor->GetClass(), true, Actor);
			}
		}
	}
	GEditor->EndTransaction();
}

#undef LOCTEXT_NAMESPACE