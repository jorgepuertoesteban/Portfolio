// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_ActorsFromSelection.h"

#include "EngineUtils.h"

#include "UObject/NoExportTypes.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"

#include "Kismet2/SClassPickerDialog.h"

#include "Misc/MessageDialog.h"

#include "Camera/CameraActor.h"

#include "Components/PointLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/RectLightComponent.h"

#include "Engine/DirectionalLight.h"
#include "Engine/Light.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/PointLight.h"
#include "Engine/RectLight.h"
#include "Engine/ReflectionCapture.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMeshActor.h"

#include "Editor/EditorEngine.h"
#include "Editor.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Selection.h"
#include "JPEEditActionsLib.h"
#include "ExtractOptions.h"


#define LOCTEXT_NAMESPACE "JPEEditAction_ActorsFromSelection"


void FJPEEditAction_ActorsFromSelection::ExecuteAction()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		UE_LOG(LogTemp, Log, TEXT("Not valid World"));
		return;
	}

	GEditor->BeginTransaction(LOCTEXT("JPEEditAction_ActorsFromSelection", "Create Mesh Actors from Actor Class"));

	FExtractOptions Options;

	Options.bDeleteOriginal = EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("DeleteOriginal", "Do you want to delete original Actor?"));
	Options.bSpawnOneActorPerComponent = EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("SpawnOneActorPerComponent ", "Do you want to Spawn one Actor per Component?"));
	Options.bUseLevelNameAsRootFolder = EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("Use Level as Folder ", "Use level name as Root Folder?"));
	Options.bReplaceAlreadyExtracted = EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("Replace already extracted", "Replace already extracted"));

	USelection* SelectedActors = GEditor->GetSelectedActors();

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AActor* Actor = Cast<AActor>(*Iter);
		if (!Actor)
		{
			continue;
		}

		bool bIsExtracted  = FJPEEditActionsLib::SpawnActorsFromComponents(Actor, World, Actor->GetClass(), Options);
		if (bIsExtracted  && Options.bDeleteOriginal)
		{
			Actor->Destroy();
		}
	}

	GEditor->EndTransaction();
}

#undef LOCTEXT_NAMESPACE