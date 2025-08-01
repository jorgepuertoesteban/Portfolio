// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_HasConstructorComponents.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"

#include "EditActions/JPEEditActionsLib.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_HasConstructorComponents"


void FJPEEditAction_HasConstructorComponents::ExecuteAction()
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	TSet<UClass*> ClassesAlreadySeen;

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* Actor = Cast<AActor>(*Iter))
		{
			if (ClassesAlreadySeen.Contains(Actor->GetClass()))
			{
				continue;
			}

			if (FJPEEditActionsLib::HasConstructorComponents(Actor))
			{
				ClassesAlreadySeen.Add(Actor->GetClass());

				FMessageDialog::Open(
					EAppMsgType::Ok,
					FText::Format(LOCTEXT("ConstructedComponent", "The Actor {0} has constructed Components (Skiping actors of the same class)"), FText::FromString(Actor->GetActorLabel()))
				);

			}
		}
	}
}

#undef LOCTEXT_NAMESPACE