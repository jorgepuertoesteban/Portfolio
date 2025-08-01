// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_CountSelectedActors.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_CountSelectedActors"


void FJPEEditAction_CountSelectedActors::ExecuteAction()
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	FMessageDialog::Open(
		EAppMsgType::Ok,
		FText::Format(LOCTEXT("ActorsCount", "Selected Actors: '{0}'"), FText::AsNumber(SelectedActors->Num()))
	);	
}

#undef LOCTEXT_NAMESPACE