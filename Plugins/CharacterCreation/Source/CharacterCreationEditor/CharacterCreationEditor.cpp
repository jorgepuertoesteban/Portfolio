// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreationEditor.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"

#include "TypeActions/CharacterCreatorTypeActions.h"

#include "TypeActions/CharacterCreatorAttributeTypeActions.h"
#include "TypeActions/CharacterCreatorPoseAttributeTypeActions.h"
#include "TypeActions/CharacterCreatorAttributesSetTypeActions.h"

#include "TypeActions/CharacterCreatorMatAttributeTypeActions.h"
#include "TypeActions/CharacterCreatorTextureMatAttributeTypeActions.h"
#include "TypeActions/CharacterCreatorVectorMatAttributeTypeActions.h"
#include "TypeActions/CharacterCreatorMatAttributesSetTypeActions.h"

#include "TypeActions/CharacterCreatorGroomTypeActions.h"
#include "TypeActions/CharacterCreatorGroomsSetTypeActions.h"
#include "TypeActions/CharacterCreatorColorsSetTypeActions.h"
//#include "TypeActions/CharacterCreatorGroomSlotTypeActions.h"

#include "TypeActions/CharacterCreatorOutfitsSetTypeActions.h"
#include "TypeActions/CharacterCreatorAttachedMeshTypeActions.h"
#include "TypeActions/CharacterCreatorOutfitTypeActions.h"
#include "TypeActions/CharacterCreatorOutfitSlotTypeActions.h"
#include "TypeActions/CharacterCreatorModelTypeActions.h"

#include "CharacterCreationCommands.h"
#include "EditActions/CharacterCreationEditActions.h"

#include "Framework/Commands/UICommandList.h"
#include "CharacterCreationStyle.h"
#include "CharacterCreationCommands.h"
#include "Styling/AppStyle.h"
#include "LevelEditor.h"
#include "Private/Workbench/CCWorkBench.h"

#include "Workbench/SCCWorkbench.h"
#include "DetailCustomization/BASkeletalMeshDetailCustomization.h"
#include "PropertyEditorModule.h"
#include "Engine/SkeletalMesh.h"

#include "ToolMenus.h"
#include "SkeletalMeshToolMenuContext.h"
#include "IHasPersonaToolkit.h"
#include "IPersonaToolkit.h"
#include "SkeletalMeshTools/SReferencePoseOptionsDialog.h"
#include "CharacterCreationEditorLibrary.h"
#include "Templates/SharedPointer.h"
#include "IEditableSkeleton.h"
#include "Private/SkeletalMeshTools/CCSkeletalMeshTools.h"
#include "ISkeletalMeshEditor.h"
#include "CharacterCreatorOutfit.h"
#include "CharacterCreatorGroom.h"
#include "Thumbnails/CharacterCreatorOutfitThumbnail.h"
#include "Thumbnails/CharacterCreatorGroomThumbnail.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationEditorModule"

class FCharacterCreationEditorModule : public ICharacterCreationEditorModule
{

private:


	TSharedPtr<class FUICommandList> PluginCommands;
	EAssetTypeCategories::Type AssetCategoryBit = EAssetTypeCategories::None;

	/** All created asset type actions.  Cached here to unregister them during shutdown. */
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
	TArray<FName> RegisteredComponentClassNames;
	TArray<FName> CustomLayoutClassNames;
	TArray<FName> CustomPropertyTypeLayoutNames;

	FCharacterCreationEditActions CharacterCreationEditActions;

	// Handle for the extender delegate we created
	FDelegateHandle SkelMeshEditorExtenderHandle;

	FCCSkeletalMeshTools CCSkeletalMeshTools;

public:

	virtual TSharedPtr<class FUICommandList> GetPluginCommands() override
	{
		return PluginCommands;
	}

	virtual EAssetTypeCategories::Type GetAssetCategoryBit() const override
	{
		return AssetCategoryBit;
	}

public:

	virtual void StartupModule() override
	{
		//Call this to make sure AnimGraph module is setup
		FModuleManager::Get().LoadModuleChecked(TEXT("Persona"));

		FCCWorkbench::Register();

		CCSkeletalMeshTools.Register();


		FCoreDelegates::OnPostEngineInit.AddRaw(this, &FCharacterCreationEditorModule::OnPostEngineInit);//Delegate for OnPostEngineInit

		RegisterCommands();
		InitializeStyles();
		CreateExtensions();

		PluginCommands = MakeShareable(new FUICommandList);
		PluginCommands->MapAction
		(
			FCharacterCreationCommands::Get().OpenCharacterCreationEditorAction,
			FExecuteAction::CreateStatic(&FCCWorkbench::Launch)
		);

		CharacterCreationEditActions.Register();


		/*UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCharacterCreationEditorModule::RegisterMenus));*/
		{
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
			AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("JPEEditor")), LOCTEXT("JPEEditor", "JPE"));

			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorTypeActions(AssetCategoryBit)));

			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorAttributeTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorPoseAttributeTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorAttributesSetTypeActions(AssetCategoryBit)));

			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorMatAttributeTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorTextureMatAttributeTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorVectorMatAttributeTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorMatAttributesSetTypeActions(AssetCategoryBit)));

			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorGroomTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorGroomsSetTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorColorsSetTypeActions(AssetCategoryBit)));
			//RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorOutfitSlotTypeActions(AssetCategoryBit)));

			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorAttachedMeshTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorOutfitTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorOutfitsSetTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorOutfitSlotTypeActions(AssetCategoryBit)));
			RegisterAssetTypeAction(AssetTools, MakeShareable(new FCharacterCreatorModelTypeActions(AssetCategoryBit)));

			//Publish here asset actions
		}

		// Register the details customization
		{
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

			RegisterCustomClassLayout<USkeletalMesh, FBASkeletalMeshDetailCustomization>(PropertyEditorModule);
			PropertyEditorModule.NotifyCustomizationModuleChanged();
		}

		UThumbnailManager::Get().RegisterCustomRenderer(UCharacterCreatorOutfit::StaticClass(), UCharacterCreatorOutfitThumbnail::StaticClass());
		UThumbnailManager::Get().RegisterCustomRenderer(UCharacterCreatorGroom::StaticClass(), UCharacterCreatorGroomThumbnail::StaticClass());


	}

	void OnPostEngineInit()
	{
		//RegisterComponentVisualizer<UPlaceableMarkerComponent, FPlaceableMarkerVisualizer>();
	}

	virtual void ShutdownModule() override
	{
		FCoreDelegates::OnPostEngineInit.RemoveAll(this);
		UnregisterCommands();
		ShutdownStyles();
		UnregisterAssetTypeActions();
	}

private:

	static void RegisterCommands()
	{
		FCharacterCreationCommands::Register();
	}

	static void UnregisterCommands()
	{
		FCharacterCreationCommands::Unregister();
	}

	static void InitializeStyles()
	{
		FCharacterCreationStyle::Initialize();
	}

	static void ShutdownStyles()
	{
		FCharacterCreationStyle::Shutdown();
	}

	void CreateExtensions()
	{
		//Place to create the different extensions for the plugin
	}

	template<typename TClass, typename TCustomization>
	void RegisterCustomClassLayout(FPropertyEditorModule& PropertyEditorModule)
	{
		const FName ClassName = TClass::StaticClass()->GetFName();
		PropertyEditorModule.RegisterCustomClassLayout(ClassName, FOnGetDetailCustomizationInstance::CreateStatic(&TCustomization::MakeInstance));
		CustomLayoutClassNames.Add(ClassName);
	}

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}

	void UnregisterAssetTypeActions()
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}
		CreatedAssetTypeActions.Empty();
	}

	//void RegisterMenus()
	//{
	//	FToolMenuOwnerScoped OwnerScoped(this);

	//	UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("AssetEditor.SkeletalMeshEditor.ToolBar");
	//	{
	//		FToolMenuSection& Section = Toolbar->FindOrAddSection("SkeletalMesh");
	//		Section.AddDynamicEntry("CharacterCreationSkeletal", FNewToolMenuSectionDelegate::CreateLambda(
	//			[this](FToolMenuSection& InSection)
	//			{
	//				USkeletalMeshToolMenuContext* Context = InSection.FindContext<USkeletalMeshToolMenuContext>();
	//				if (Context && Context->SkeletalMeshEditor.IsValid())
	//				{
	//					InSection.AddEntry(
	//						FToolMenuEntry::InitToolBarButton(
	//							FCharacterCreationCommands::Get().OpenSkeletalDialog,
	//							LOCTEXT("CharacterCreationSkeletalDialog", "Character Creation Skeletal Tools"),
	//							LOCTEXT("CharacterCreationSkeletalDialogTooltip", "Opens the Skeletal by opening a dialog"),
	//							FSlateIcon(FAppStyle::GetAppStyleSetName(), "SkeletalMeshEditor.MeshSectionSelection")
	//						)
	//					);
	//				}
	//			}));
	//	}
	//}

	TSharedRef<FExtender> ExtendSkelMeshEditorToolbar(const TSharedRef<FUICommandList> InCommandList, TSharedRef<ISkeletalMeshEditor> InSkeletalMeshEditor)
	{
		// Add toolbar extender
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

		TWeakPtr<ISkeletalMeshEditor> Ptr(InSkeletalMeshEditor);

		InCommandList->MapAction(
			FCharacterCreationCommands::Get().OpenSkeletalDialog,
			FExecuteAction::CreateRaw(this, &FCharacterCreationEditorModule::OpenSkeletalDialog, TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor))
		);

		return ToolbarExtender.ToSharedRef();
	}


	void OpenSkeletalDialog(TWeakPtr<ISkeletalMeshEditor> Ed) const
	{
		if (Ed.IsValid())
		{
			UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent();
			if (Component)
			{
				TSharedRef<SReferencePoseOptionsDialog> OptionsModal = SNew(SReferencePoseOptionsDialog).CurrentMesh(Component);

				// this blocks until the modal is closed
				const FRefUpdatePoseOptions Options = OptionsModal->ShowModal();

				if (Options.bUpdateMesh)
				{
					const FText Message = LOCTEXT("UpdateMeshGeoConfirm", "You have selected to update the mesh geometry, are you sure you want to do this? This cannot be undone without source control.");
					const EAppReturnType::Type RetVal = FMessageDialog::Open(EAppMsgType::YesNo, Message);
					if (RetVal == EAppReturnType::No)
					{
						return;
					}
				}

				if (Options.UpdateType == ERefPoseOptionsReturnType::CurrentPose)
				{
					UCharacterCreationEditorLibrary::UpdateSkelMeshReferencePoseFromCurrent(
						Component,
						Options.bUpdateReferenceSkeleton,
						Options.bUpdateMesh,
						Options.bRotations,
						Options.bLocations,
						Options.bScale
					);
					Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
				}
				else if (Options.UpdateType == ERefPoseOptionsReturnType::SelectedAsset)
				{
					const TWeakObjectPtr<UObject> Asset = OptionsModal->GetSelectedAsset();
					check(Asset.IsValid());
					check(Asset.Get() != nullptr);
					UCharacterCreationEditorLibrary::UpdateSkelMeshReferencePoseFromSource(
						Component,
						Asset.Get(),
						Options.bUpdateReferenceSkeleton,
						Options.bUpdateMesh,
						Options.bRotations,
						Options.bLocations,
						Options.bScale
					);
					Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
				}
			}
		}
	}
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCharacterCreationEditorModule, CharacterCreationEditor)