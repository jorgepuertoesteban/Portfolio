#include "EditorToolkits/CharacterCreatorToolkit.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Docking/SDockTab.h"
#include "Internationalization/Text.h"
#include "Framework/Docking/TabManager.h"
#include "CharacterCreator.h"
#include "EditorViewports/SCharacterCreatorViewport.h"

FName ViewportTab = FName(TEXT("ViewportTab"));
FName DetailsTab = FName(TEXT("DetailsTab"));

void FCharacterCreatorToolkit::Initialize(const TSharedPtr<IToolkitHost>& InitToolkitHost, UCharacterCreator* Style)
{
	CharacterCreator = Style;
	
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(FName(TEXT("CustomEditorLayout")))
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Horizontal)
		->Split
		(
			FTabManager::NewStack()
         	->SetSizeCoefficient(.5f)
         	->AddTab(ViewportTab, ETabState::OpenedTab)
         	->SetHideTabWell(true)
		)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Vertical)
			->SetSizeCoefficient(.5f)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(.8f)
				->AddTab(DetailsTab, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		)
	);
	
	InitAssetEditor(EToolkitMode::Standalone, InitToolkitHost, GetToolkitFName(), Layout, true, true, Style);
}

void FCharacterCreatorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("CustomConfigEditor"));

	// VIEWPORT
	InTabManager->RegisterTabSpawner(ViewportTab, FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SCharacterCreatorViewport)
				.EditingObject(CharacterCreator.Get())
			]
		];
	}))
	.SetDisplayName(INVTEXT("Viewport"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
		

	// DETAILS VIEW
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	const TSharedRef<IDetailsView> DetailsView = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{CharacterCreator.Get()});
	
	InTabManager->RegisterTabSpawner(DetailsTab, FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		.TabRole(PanelTab)
		[
			DetailsView
		];
	}))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FCharacterCreatorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	CharacterCreator = nullptr;

	TabManagerRef->UnregisterAllTabSpawners();
	
	FAssetEditorToolkit::UnregisterTabSpawners(TabManagerRef);
}

TSharedRef<FCharacterCreatorToolkit> FCharacterCreatorToolkit::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& InObjects)
{
	TSharedRef<FCharacterCreatorToolkit> NewEditor(new FCharacterCreatorToolkit());
	NewEditor->Initialize(InitToolkitHost, Cast<UCharacterCreator>(InObjects[0]));
	return NewEditor;
}