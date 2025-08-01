// Created by Jorge Puerto. All Rights Reserved.


#include "Workbench/SCCWorkbenchViewport.h"

#include "EngineUtils.h"
#include "PreviewScene.h"
#include "Widgets/Docking/SDockTab.h"
#include "AdvancedPreviewScene.h"

DEFINE_LOG_CATEGORY_STATIC(LogDomainEdViewport, Log, All);
#define LOCTEXT_NAMESPACE "CCWorkbenchViewport"

//////////////////////////// SCCWorkbenchViewport ////////////////////////////
void SCCWorkbenchViewport::Construct(const FArguments& InArgs)
{
	//PreviewScene = MakeShareable(new FPreviewScene);
	{
		FAdvancedPreviewScene::ConstructionValues CVS;
		CVS.bCreatePhysicsScene = false;
		CVS.LightBrightness = 3;
		CVS.SkyBrightness = 1;
		PreviewScene = MakeShareable(new FAdvancedPreviewScene(CVS));
	}


	//UWorld* World = PreviewScene->GetWorld();

	//PreviewScene->DirectionalLight->SetMobility(EComponentMobility::Movable);
	//PreviewScene->DirectionalLight->CastShadows = true;
	//PreviewScene->DirectionalLight->CastStaticShadows = true;
	//PreviewScene->DirectionalLight->CastDynamicShadows = true;
	//PreviewScene->DirectionalLight->SetIntensity(1.0f);

	//UTextureCube* Cubemap = Cast<UTextureCube>(
	//	StaticLoadObject(UTextureCube::StaticClass(), nullptr, TEXT("/Engine/MapTemplates/daylight")));
	//PreviewScene->SetSkyCubemap(Cubemap);
	//PreviewScene->SetSkyBrightness(0.5f);
	//PreviewScene->UpdateCaptureContents();


	SEditorViewport::Construct(SEditorViewport::FArguments());

	BindCommands();
}

SCCWorkbenchViewport::~SCCWorkbenchViewport()
{
	if (EditorViewportClient.IsValid())
	{
		EditorViewportClient->Viewport = nullptr;
	}
}

void SCCWorkbenchViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
}

UWorld* SCCWorkbenchViewport::GetWorld() const
{
	return PreviewScene->GetWorld();
}

FFDViewportActorMouseEvent& SCCWorkbenchViewport::GetActorSelectionChanged()
{
	return EditorViewportClient->GetActorSelectionChanged();
}

FFDViewportActorMouseEvent& SCCWorkbenchViewport::GetActorDoubleClicked()
{
	return EditorViewportClient->GetActorDoubleClicked();
}

FString SCCWorkbenchViewport::GetReferencerName() const
{
	return TEXT("SCCWorkbenchViewportReferenceName");
}

bool SCCWorkbenchViewport::GetReferencerPropertyName(UObject* Object, FString& OutPropertyName) const
{
	OutPropertyName = TEXT("SCCWorkbenchViewportReferenceName");
	return true;
}

TSharedRef<FEditorViewportClient> SCCWorkbenchViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new SCCWorkbenchViewportClient(*PreviewScene, SharedThis(this)));
	EditorViewportClient->SetRealtime(true);
	EditorViewportClient->VisibilityDelegate.BindSP(this, &SCCWorkbenchViewport::IsVisible);
	return EditorViewportClient.ToSharedRef();
}

bool SCCWorkbenchViewport::IsVisible() const
{
	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground());
}


//////////////////////////// SCCWorkbenchViewportClient ////////////////////////////


SCCWorkbenchViewportClient::SCCWorkbenchViewportClient(FPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, InEditorViewport)
{
	//static const float ViewportGridSize = 2048.0f;
	//static const int32 ViewportCellSize = 16;

	// Setup defaults for the common draw helper.
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(160, 160, 160);
	DrawHelper.GridColorMajor = FColor(144, 144, 144);
	DrawHelper.GridColorMinor = FColor(128, 128, 128);
	//DrawHelper.PerspectiveGridSize = ViewportGridSize;
	//DrawHelper.NumCells = DrawHelper.PerspectiveGridSize / (ViewportCellSize * 2);

	FEditorViewportClient::SetViewMode(EViewModeIndex::VMI_Lit);

	EngineShowFlags.SetSnap(false);
	EngineShowFlags.CompositeEditorPrimitives = true;
	OverrideNearClipPlane(1.0f);
	bUsingOrbitCamera = true;

	const FVector CamLocation(200, 200, 200);

	// Set the initial camera position
	const FRotator OrbitRotation(-45, 100, 0);
	SetCameraSetup(
		FVector::ZeroVector,
		OrbitRotation,
		FVector(0.0f, 100, 0.0f),
		FVector::ZeroVector,
		CamLocation,
		FRotator(0, 0, 0)
	);
}

void SCCWorkbenchViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread) {
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}
void SCCWorkbenchViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	FEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);

	AActor* SelectedActor = nullptr;

	if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
	{
		HActor* ActorHit = static_cast<HActor*>(HitProxy);
		SelectedActor = ActorHit ? ActorHit->Actor : nullptr;
	}
	ActorSelectionChanged.ExecuteIfBound(SelectedActor);

	if (Event == IE_DoubleClick && SelectedActor)
	{
		ActorDoubleClicked.ExecuteIfBound(SelectedActor);
	}
}

void SCCWorkbenchViewportClient::SetupViewForRendering(FSceneViewFamily& ViewFamily, FSceneView& View)
{
	FEditorViewportClient::SetupViewForRendering(ViewFamily, View);

	View.AntiAliasingMethod = AAM_FXAA;
}

#undef LOCTEXT_NAMESPACE




//
//#pragma once
//
//#include "Workbench/SCCWorkbenchViewport.h"
//#include "PreviewScene.h"
//#include "EditorViewportClient.h"
//
//#include "Engine/TextureCube.h"
//
//#include "Components/DirectionalLightComponent.h"
//#include "Templates/SharedPointer.h"
//
//
//void SCCWorkbenchViewport::Construct(const FArguments& InArgs)
//{
//	PreviewScene = MakeShareable(new FPreviewScene);
//
//	SEditorViewport::Construct(SEditorViewport::FArguments());
//}
//
//TSharedRef<FEditorViewportClient> SCCWorkbenchViewport::MakeEditorViewportClient()
//{
//	ViewportClient = MakeShareable(new FEditorViewportClient(nullptr, PreviewScene.Get(), SharedThis(this)));
//
//	//EditorViewportClient->bSetListenerPosition = false;
//	ViewportClient->SetRealtime(true);
//	//EditorViewportClient->VisibilityDelegate.BindSP(this, &SFlowPreview3DViewport::IsVisible);
//	return ViewportClient.ToSharedRef();
//}
//
//void SCCWorkbenchViewport::Construct(const FArguments& InArgs) {
//	PreviewScene = MakeShareable(new FPreviewScene);
//	SEditorViewport::Construct(SEditorViewport::FArguments());
//
//	BindCommands();
//}
//
//SCCWorkbenchViewport::~SCCWorkbenchViewport() {
//	if (EditorViewportClient.IsValid()) {
//		EditorViewportClient->Viewport = nullptr;
//	}
//}
//
//void SCCWorkbenchViewport::AddReferencedObjects(FReferenceCollector& Collector) {
//}
//
//UWorld* SCCWorkbenchViewport::GetWorld() const {
//	return PreviewScene->GetWorld();
//}
//
//FFDViewportActorMouseEvent& SCCWorkbenchViewport::GetActorSelectionChanged() {
//	return EditorViewportClient->GetActorSelectionChanged();
//}
//
//FFDViewportActorMouseEvent& SCCWorkbenchViewport::GetActorDoubleClicked() {
//	return EditorViewportClient->GetActorDoubleClicked();
//}
//
//TSharedRef<FEditorViewportClient> SCCWorkbenchViewport::MakeEditorViewportClient() {
//	EditorViewportClient = MakeShareable(new SCCWorkbenchViewportClient(*PreviewScene, SharedThis(this)));
//	EditorViewportClient->SetRealtime(true);
//	EditorViewportClient->VisibilityDelegate.BindSP(this, &SCCWorkbenchViewport::IsVisible);
//	return EditorViewportClient.ToSharedRef();
//}
//
//bool SCCWorkbenchViewport::IsVisible() const {
//	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground());
//}
