// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_DeleteNullMeshComp.h"
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"
#include "Components/StaticMeshComponent.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_DeleteNullMeshComp"


void FJPEEditAction_DeleteNullMeshComp::ExecuteAction()
{

	USelection* SelectedActors = GEditor->GetSelectedActors();

	TArray<UStaticMeshComponent*> ComponentsToDelete;

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* Actor = Cast<AActor>(*Iter))
		{
			for (UActorComponent* Comp : Actor->GetComponents())
			{
				if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
				{
					if (!MeshComp->GetStaticMesh())
					{
						ComponentsToDelete.Add(MeshComp);
					}
				}
			}
		}
	}
	for (UStaticMeshComponent* Comp : ComponentsToDelete)
	{
		Comp->DestroyComponent(false);
	}
}

#undef LOCTEXT_NAMESPACE