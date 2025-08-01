// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_RenameSelectedActors.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Launch/Resources/Version.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_RenameSelectedActors"


void FJPEEditAction_RenameSelectedActors::ExecuteAction()
{

	TSharedRef<SEditableText> EditableText = SNew(SEditableText).Text(FText::FromString("NewName")).IsReadOnly(false);

#if ENGINE_MAJOR_VERSION == 4
	SGenericDialogWidget::OpenDialog(
		LOCTEXT("FJPEEditAction_RenameSelectedActors", "Rename Selected Actors"),
		EditableText,
		SGenericDialogWidget::FArguments()
	);
#elif ENGINE_MAJOR_VERSION == 5
	SGenericDialogWidget::OpenDialog(
		LOCTEXT("FJPEEditAction_RenameSelectedActors", "Rename Selected Actors"), 
		EditableText,
		SGenericDialogWidget::FArguments(), 
		true
	);
#endif

	GEditor->BeginTransaction(LOCTEXT("JPEEditAction_RenameSelectedActors", "Rename Selected Actors"));
	FString NewName = EditableText->GetText().ToString();
	
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// Let editor know that we're about to do something that we want to undo/redo

	// For each selected actor
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* LevelActor = Cast<AActor>(*Iter))
		{
			//LevelActor->SetActorLabel(*NewName);//Doesn't make it unique so we use FActorLabelUtilities::SetActorLabelUnique
			FActorLabelUtilities::SetActorLabelUnique(LevelActor, NewName);
		}
	}

	// We're done moving actors so close transaction

	GEditor->EndTransaction();
}

#undef LOCTEXT_NAMESPACE