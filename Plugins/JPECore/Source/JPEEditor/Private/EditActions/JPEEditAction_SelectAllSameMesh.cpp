// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_SelectAllSameMesh.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_SelectAllSameMesh"


void FJPEEditAction_SelectAllSameMesh::ExecuteAction()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		UE_LOG(LogTemp, Log, TEXT("Not valid World"));
		return;
	}

	UStaticMesh* SelectedStaticMesh = nullptr;

	USelection* SelectedActors = GEditor->GetSelectedActors();

	if (SelectedActors->Num() != 1)
	{
		FMessageDialog::Open(
			EAppMsgType::Ok,
			LOCTEXT("ConstructedComponent", "Pleaes select only one Actor")
		);
		return;
	}

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* Actor = Cast<AActor>(*Iter))
		{
			for (UActorComponent* Comp : Actor->GetComponents())
			{
				if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
				{
					SelectedStaticMesh = MeshComp->GetStaticMesh();
					if (SelectedStaticMesh)
					{
						break;
					}
				}
			}
		}
	}

	SelectedActors->DeselectAll();
	SelectedActors->BeginBatchSelectOperation();
	SelectedActors->Modify();
	for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
	{
		if (AActor* Actor = Cast<AActor>(*It))
		{
			for (UActorComponent* Comp : Actor->GetComponents())
			{
				if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
				{
					if (SelectedStaticMesh == MeshComp->GetStaticMesh())
					{
						GEditor->SelectActor(Actor, true, true, true, true);
						continue;
					}
				}
			}
		}
	}
	SelectedActors->EndBatchSelectOperation();
	GEditor->NoteSelectionChange();
}

#undef LOCTEXT_NAMESPACE