// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_MeshFromBlueprint.h"

#include "EngineUtils.h"

#include "UObject/NoExportTypes.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"

#include "Kismet2/SClassPickerDialog.h"

#include "Misc/MessageDialog.h"

#include "Camera/CameraActor.h"

#include "Engine/Light.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/ReflectionCapture.h"
#include "Engine/StaticMeshActor.h"

#include "Editor/EditorEngine.h"
#include "Editor.h"
#include "JPEEditActionsLib.h"
#include "SMeshFromBlueprint.h"
#include "Runtime/Launch/Resources/Version.h"

const TSet<FString> FJPEEditAction_MeshFromBlueprint::IgnoreClassNames =
{
	"TextRenderActor",
	"WorldSettings",
	"SkeletalMeshActor",
	"Brush",
	"DefaultPhysicsVolume",
	"GameplayDebuggerPlayerManager",
	"AbstractNavData",
	"InstancedFoliageActor",
	"LandscapeGizmoActiveActor",
	"Landscape",
	"BP_Splines_C",
	"DecalActor",
	"LightmassImportanceVolume",
	"AtmosphericFog",
	"BP_Sky_Sphere_C",
	"SkyLight",
	"JPESpawnerActor",
	"Emitter",
	"GroupActor",
	"ExponentialHeightFog",
	"PostProcessVolume",
	"CullDistanceVolume",
	"WorldSettings",
	"Actor"
};

const TSet<UClass*> FJPEEditAction_MeshFromBlueprint::IgnoreClasses =
{
	AStaticMeshActor::StaticClass(),
	ALight::StaticClass(),
	ACameraActor::StaticClass(),
	AReflectionCapture::StaticClass(),
	ALevelScriptActor::StaticClass()
};

class FAssetClassParentFilter : public IClassViewerFilter
{
public:
	/** All children of these classes will be included unless filtered out by another setting. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

#define LOCTEXT_NAMESPACE "JPEEditAction_MeshFromBlueprint"



bool DeleteOriginal(const FExtractOptions& ExtractOptions)
{
	//if skip is false shows a message to ask otherwise returns ExtractOptions.bDeleteOriginal 
	return (ExtractOptions.bSkipDeleteConfirmation || EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("DeleteOriginal", "Do you want to delete original Actor?")))
		&& (ExtractOptions.bSkipDeleteConfirmation && ExtractOptions.bDeleteOriginal);
}

void FJPEEditAction_MeshFromBlueprint::ExecuteAction()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		UE_LOG(LogTemp, Log, TEXT("Not valid World"));
		return;
	}

	GEditor->BeginTransaction(LOCTEXT("JPEEditAction_MeshFromBlueprint", "Create Mesh Actors from Actor Class"));

	TSharedRef<SMeshFromBlueprint> OptionsModal = SNew(SMeshFromBlueprint);
	// this blocks until the modal is closed
	FExtractOptions ExtractOptions;
	bool bAccepted = OptionsModal->ShowModal(ExtractOptions);

	if (!bAccepted)
	{
		return;
	}

	if (ExtractOptions.bIterateAllActors)
	{
		TSet<UClass*> ClassesAlreadySeen;
		TSet<AActor*> ActorsToDestroy;

		for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;
			{
				if (!Actor)
				{
					continue;
				}
				if (FJPEEditAction_MeshFromBlueprint::IgnoreActor(Actor))
				{
					continue;
				}
				bool bIsAlreadyInSet = false;
				ClassesAlreadySeen.Add(Actor->GetClass(), &bIsAlreadyInSet);
				if (bIsAlreadyInSet) continue;
			}

			if (ExtractOptions.bOnlyConstructorComponentsActors && FJPEEditActionsLib::HasConstructorComponents(Actor))
			{
				continue;
			}

			FName const ClassName = Actor->GetClass()->GetFName();
			
			bool const bExtractThisClass =
				ExtractOptions.bSkipActorReplacementConfirmation ||
				EAppReturnType::Type::Yes == FMessageDialog::Open(
					EAppMsgType::YesNo,
					FText::Format(LOCTEXT("LoadingClass", "Replace Actors of class: '{0}'"), FText::FromName(ClassName))
				);

			if (!bExtractThisClass)
			{
				continue;
			}

			bool const bDeleteOriginal = DeleteOriginal(ExtractOptions);

			for (TActorIterator<AActor> ChosenActorIt(World, Actor->GetClass()); ChosenActorIt; ++ChosenActorIt)
			{
				AActor* ChosenActor = *ChosenActorIt;

				bool bIsExtracted = FJPEEditActionsLib::SpawnActorsFromComponents(ChosenActor, World, ChosenActor->GetClass(), ExtractOptions);

				if (bIsExtracted && bDeleteOriginal)
				{
					ActorsToDestroy.Add(ChosenActor);
				}
			}
		}
		for (AActor* ActorToDestroy : ActorsToDestroy)
		{
			ActorToDestroy->Destroy();
		}
	}
	else do {

		FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
		FClassViewerInitializationOptions Options;
		Options.Mode = EClassViewerMode::ClassPicker;

		TSharedPtr<FAssetClassParentFilter> Filter = MakeShared<FAssetClassParentFilter>();
		Filter->AllowedChildrenOfClasses.Add(AActor::StaticClass());
#if ENGINE_MAJOR_VERSION == 4
		Options.ClassFilter = Filter;
#elif ENGINE_MAJOR_VERSION == 5
		Options.ClassFilters.Add(Filter.ToSharedRef());
#endif

		const FText TitleText = NSLOCTEXT("AudioModulation", "CreateSoundModulationParameterOptions", "Select Parameter Class");
		UClass* ChosenClass = nullptr;

		const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, AActor::StaticClass());

		if (bPressedOk)
		{
			bool const bDeleteOriginal = DeleteOriginal(ExtractOptions);

			for (TActorIterator<AActor> It(World, ChosenClass); It; ++It)
			{
				AActor* Actor = *It;
				FJPEEditActionsLib::SpawnActorsFromComponents(Actor, World, ChosenClass, ExtractOptions);

				if (bDeleteOriginal)
				{
					Actor->Destroy();
				}
			}
		}
	} while (EAppReturnType::Type::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("Continue_Extracting", "Do you want to continue Extracting?")));

	GEditor->EndTransaction();
}


bool FJPEEditAction_MeshFromBlueprint::IgnoreActor(AActor* Actor)
{
	if (IgnoreClassNames.Contains(Actor->GetClass()->GetName()))
	{
		return true;
	}
	for (UClass* Class : IgnoreClasses)
	{
		if (Actor->IsA(Class))
		{
			return true;
		}
	}
	return false;
}


#undef LOCTEXT_NAMESPACE