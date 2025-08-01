#include "EditorViewports/SCharacterCreatorViewport.h"
#include "AdvancedPreviewScene.h"
#include "CharacterCreator.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterCreatorViewportHelper.h"
#include "Components/CharacterCreatorComponent.h"

SCharacterCreatorViewport::~SCharacterCreatorViewport()
{
	if (InputObjectPropertyChangedHandle.IsValid())
	{
		FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(InputObjectPropertyChangedHandle);
	}
}

void SCharacterCreatorViewport::Construct(const FArguments& InArgs)
{
	// We need to create a new Scene before constructing this viewport. Otherwise, it will default to grabbing the one from the main World in the Editor
	AdvancedPreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));

	SEditorViewport::Construct(SEditorViewport::FArguments());
	
	CharacterCreator = InArgs._EditingObject;
	if (!CharacterCreator.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Editing asset is not valid"));
		return;
	}
	ACharacterCreatorViewportHelper* PreviewCharacterCreator = GetWorld()->SpawnActor<ACharacterCreatorViewportHelper>();

	InputObjectPropertyChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddLambda([this, PreviewCharacterCreator](UObject* Object, const FPropertyChangedEvent& PropertyChangedEvent)
		{
			PreviewCharacterCreator->GetCharacterCreatorComponent()->SetCharacterCreator(CharacterCreator.Get());			
		});


	LevelViewportClient->SetViewLocationForOrbiting(FVector(0.f, 0.f, 40.f));
	LevelViewportClient->SetViewLocation(FVector(0.f, 150.f, 40.f));
	LevelViewportClient->SetViewRotation(UKismetMathLibrary::FindLookAtRotation(LevelViewportClient->GetViewLocation(), FVector(0.f, 0.f, 40.f)));

	PreviewCharacterCreator->GetCharacterCreatorComponent()->SetCharacterCreator(CharacterCreator.Get());
	AdvancedPreviewScene->SetFloorVisibility(false, true);
	AdvancedPreviewScene->SetEnvironmentVisibility(false, true);

}

TSharedRef<FEditorViewportClient> SCharacterCreatorViewport::MakeEditorViewportClient()
{
	LevelViewportClient = MakeShareable(new FEditorViewportClient(nullptr, AdvancedPreviewScene.Get(), SharedThis(this)));
	
	LevelViewportClient->ViewportType = LVT_Perspective;
	LevelViewportClient->bSetListenerPosition = false;
	LevelViewportClient->SetRealtime(true);

	

	return LevelViewportClient.ToSharedRef();
}