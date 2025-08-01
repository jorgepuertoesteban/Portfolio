// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetEditorModeManager.h"
#include "SEditorViewport.h"

DECLARE_DELEGATE_OneParam(FFDViewportActorMouseEvent, AActor*);


class CHARACTERCREATIONEDITOR_API SCCWorkbenchViewport : public SEditorViewport, public FGCObject
{

protected:
	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;
	/** Level viewport client */
	TSharedPtr<class SCCWorkbenchViewportClient> EditorViewportClient;
	/** The scene for this viewport. */
	TSharedPtr<FPreviewScene> PreviewScene;

public:

	SLATE_BEGIN_ARGS(SCCWorkbenchViewport) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);
	~SCCWorkbenchViewport();

	// FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FGCObject interface

	///** Set the parent tab of the viewport for determining visibility */
	//void SetParentTab(TSharedRef<SDockTab> InParentTab) { ParentTab = InParentTab; }

	virtual UWorld* GetWorld() const override;
	TSharedPtr<FPreviewScene> GetPreviewScene() const { return PreviewScene; }

	FFDViewportActorMouseEvent& GetActorSelectionChanged();
	FFDViewportActorMouseEvent& GetActorDoubleClicked();


	FString GetReferencerName() const override;


	bool GetReferencerPropertyName(UObject* Object, FString& OutPropertyName) const override;

protected:

	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual EVisibility OnGetViewportContentVisibility() const override { return EVisibility::Visible; }

private:
	/** Determines the visibility of the viewport. */
	virtual bool IsVisible() const override;
};


class CHARACTERCREATIONEDITOR_API SCCWorkbenchViewportClient : public FEditorViewportClient, public TSharedFromThis<SCCWorkbenchViewportClient>
{

public:
	
	SCCWorkbenchViewportClient(FPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewport);
	FFDViewportActorMouseEvent& GetActorSelectionChanged() { return ActorSelectionChanged; }
	FFDViewportActorMouseEvent& GetActorDoubleClicked() { return ActorDoubleClicked; }

	// FEditorViewportClient interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
	virtual void SetupViewForRendering(FSceneViewFamily& ViewFamily, FSceneView& View) override;
	// End of FEditorViewportClient interface


private:

	FFDViewportActorMouseEvent ActorSelectionChanged;
	FFDViewportActorMouseEvent ActorDoubleClicked;
};





//
//#pragma once
//
//#include "Templates/SharedPointer.h"
//#include "SEditorViewport.h"
//#include "EditorViewportClient.h"
//
//class FPreviewScene;
//class FEditorViewportClient;
//
//class CHARACTERCREATIONEDITOR_API SCCWorkbenchViewport : public SEditorViewport
//{
//public:
//
//	SLATE_BEGIN_ARGS(SCCWorkbenchViewport) {}
//	SLATE_END_ARGS()
//
//private:
//
//	TSharedPtr<FEditorViewportClient> ViewportClient;
//
//	/** The scene for this viewport. */
//	TSharedPtr<FPreviewScene> PreviewScene;
//
//public:
//
//    ~SCCWorkbenchViewport();
//
//    // FGCObject interface
//    virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
//    // End of FGCObject interface
//
//    /** Set the parent tab of the viewport for determining visibility */
//    void SetParentTab(TSharedRef<SDockTab> InParentTab) { ParentTab = InParentTab; }
//
//    virtual UWorld* GetWorld() const override;
//    TSharedPtr<FPreviewScene> GetPreviewScene() const { return PreviewScene; }
//
//    FFDViewportActorMouseEvent& GetActorSelectionChanged();
//    FFDViewportActorMouseEvent& GetActorDoubleClicked();
//
//protected:
//    /** SEditorViewport interface */
//    virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
//    virtual EVisibility OnGetViewportContentVisibility() const override { return EVisibility::Visible; }
//
//private:
//    /** Determines the visibility of the viewport. */
//    virtual bool IsVisible() const override;
//
//protected:
//    /** The parent tab where this viewport resides */
//    TWeakPtr<SDockTab> ParentTab;
//
//    /** Level viewport client */
//    TSharedPtr<class SCCWorkbenchViewportClient> EditorViewportClient;
//
//    /** The scene for this viewport. */
//    TSharedPtr<FPreviewScene> PreviewScene;
//
//	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
//
//	void Construct(const FArguments& InArgs);
//};