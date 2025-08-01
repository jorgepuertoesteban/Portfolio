#include "SkeletalMeshTools/CCSkeletalMeshTools.h"
#include "Modules/ModuleManager.h"
#include "ISkeletalMeshEditorModule.h"
#include "SkeletalMeshTools/CCSkeletalMeshToolsCommands.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "SkeletalMeshToolMenuContext.h"
#include "SkeletalMeshTools/SReferencePoseOptionsDialog.h"
#include "CharacterCreationEditorLibrary.h"
#include "SCopyBonesLocationDialog.h"
#include "SSkeletonWidget.h"
#include "Components/SkinnedMeshComponent.h"
#include "SkeletalMeshTools/DlgMergeSkeleton.h"
#include "AssetNotifications.h"
#include "IPersonaToolkit.h"
#include "SkeletalMeshTools/CCSkeletalMeshTools.h"

#include "IEditableSkeleton.h"
#include "Animation/DebugSkelMeshComponent.h"

#define LOCTEXT_NAMESPACE "FCCSkeletalMeshTools"

void FCCSkeletalMeshTools::Register()
{
	FCCSkeletalMeshToolsCommands::Register();
	////TODO: THIS LINE CRASHES -> GEditor->RegisterForUndo(this);

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateLambda(
			[this]()
			{
				FToolMenuOwnerScoped OwnerScoped(this);
				UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("AssetEditor.SkeletalMeshEditor.ToolBar");
				FToolMenuSection& Section = Toolbar->FindOrAddSection("SkeletalMesh");
				Section.AddDynamicEntry(
					"CharacterCreation",
					FNewToolMenuSectionDelegate::CreateLambda(
						[this](FToolMenuSection& InSection)
						{
							USkeletalMeshToolMenuContext* Context = InSection.FindContext<USkeletalMeshToolMenuContext>();
							if (Context && Context->SkeletalMeshEditor.IsValid())
							{
								InSection.AddEntry(
									FToolMenuEntry::InitComboButton(
										"CharacterCreation_ComboButton",
										FUIAction(),
										FNewToolMenuDelegate::CreateLambda(
											[](UToolMenu* InSubMenu)
											{
												FToolMenuSection& Section = InSubMenu->FindOrAddSection("CharacterCreation_ComboButton_Section");
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().OpenMeshEditor));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().OpenPoseMatcher));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().CopyBonesLocation));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().ResetModifiedBones));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().CopyBonesToPreview));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().DeleteExtraBones));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().SaveCurrentReferencePose));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().BakeCurrentGeometry));
												Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().ForceAssignSkeleton));
											}
										),
										//BEWARE OF AUTO FORMATTING 
												LOCTEXT("CharacterCreationSkeletal_Label", "Character Creation"),
												LOCTEXT("CharacterCreationSkeletal_Tooltip", "Character Creation Tools"),
												FSlateIcon(
													FAppStyle::GetAppStyleSetName(),
													"SkeletalMeshEditor.MeshSectionSelection"
												)
												)
									// END BEWARE OF AUTO FORMATTING 
												);
							}
						}
					)
				);
				//UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("SkeletonTreeBoneReductionForLOD");
				//FToolMenuSection& Section = Toolbar->FindOrAddSection("SkeletalMesh");
				//Section.AddDynamicEntry(
				//	"CharacterCreation",
				//	FNewToolMenuSectionDelegate::CreateLambda(
				//		[this](FToolMenuSection& InSection)
				//		{
				//			USkeletalMeshToolMenuContext* Context = InSection.FindContext<USkeletalMeshToolMenuContext>();
				//			if (Context && Context->SkeletalMeshEditor.IsValid())
				//			{
				//				InSection.AddEntry(
				//					FToolMenuEntry::InitComboButton(
				//						"CharacterCreation_ComboButton",
				//						FUIAction(),
				//						FNewToolMenuDelegate::CreateLambda(
				//							[](UToolMenu* InSubMenu)
				//							{
				//								FToolMenuSection& Section = InSubMenu->FindOrAddSection("CharacterCreation_ComboButton_Section");
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().OpenMeshEditor));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().OpenPoseMatcher));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().CopyBonesLocation));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().ResetModifiedBones));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().CopyBonesToPreview));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().DeleteExtraBones));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().SaveCurrentReferencePose));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().BakeCurrentGeometry));
				//								Section.AddEntry(FToolMenuEntry::InitMenuEntry(FCCSkeletalMeshToolsCommands::Get().ForceAssignSkeleton));
				//							}
				//						),
				//						//BEWARE OF AUTO FORMATTING 
				//								LOCTEXT("CharacterCreationSkeletal_Label", "Character Creation"),
				//								LOCTEXT("CharacterCreationSkeletal_Tooltip", "Character Creation Tools"),
				//								FSlateIcon(
				//									FAppStyle::GetAppStyleSetName(),
				//									"SkeletalMeshEditor.MeshSectionSelection"
				//								)
				//								)
				//					// END BEWARE OF AUTO FORMATTING 
				//								);
				//			}
				//		}
				//	)
				//);
			}
		)
	);

	if (!SkelMeshEditorExtenderHandle.IsValid())
	{
		ISkeletalMeshEditorModule& SkelMeshEditorModule = FModuleManager::Get().LoadModuleChecked<ISkeletalMeshEditorModule>("SkeletalMeshEditor");
		SkelMeshEditorModule.GetMenuExtensibilityManager();

		TSharedPtr<FExtensibilityManager> MenuExtensibilityManager = SkelMeshEditorModule.GetMenuExtensibilityManager();
		TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager = SkelMeshEditorModule.GetToolBarExtensibilityManager();

		//ISkeletalMeshEditorModule::



		TArray<ISkeletalMeshEditorModule::FSkeletalMeshEditorToolbarExtender>& Extenders = SkelMeshEditorModule.GetAllSkeletalMeshEditorToolbarExtenders();
		Extenders.Add(
			ISkeletalMeshEditorModule::FSkeletalMeshEditorToolbarExtender::CreateLambda(
				[](const TSharedRef<FUICommandList> InCommandList, TSharedRef<ISkeletalMeshEditor> InSkeletalMeshEditor)->TSharedRef<FExtender>
				{
					const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

					// InCommandList->MapAction(
					// 	FCCSkeletalMeshToolsCommands::Get().OpenMeshEditor,
					// 	FExecuteAction::CreateStatic(
					// 		&FCCSkeletalMeshTools::OpenMeshEditor,
					// 		TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
					// 	)
					// );
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().OpenPoseMatcher,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::OpenPoseMatcher,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().CopyBonesLocation,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::CopyBonesLocation,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().ResetModifiedBones,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::ResetModifiedBones,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().DeleteExtraBones,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::DeleteExtraBones,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					/*
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().CopyBonesToPreview,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::CopyBonesToPreview,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().SaveCurrentReferencePose,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::SaveCurrentReferencePose,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					*/
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().BakeCurrentGeometry,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::BakeCurrentGeometry,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					InCommandList->MapAction(
						FCCSkeletalMeshToolsCommands::Get().ForceAssignSkeleton,
						FExecuteAction::CreateStatic(
							&FCCSkeletalMeshTools::ForceAssignSkeleton,
							TWeakPtr<ISkeletalMeshEditor>(InSkeletalMeshEditor)
						)
					);
					return ToolbarExtender.ToSharedRef();
				}
			)
		);
		SkelMeshEditorExtenderHandle = Extenders.Last().GetHandle();
	}

	//MenuBuilder.BeginSection("SkeletonTreeBoneReductionForLOD", LOCTEXT("BoneReductionHeader", "LOD Bone Reduction"));
	//{
	//	MenuBuilder.AddSubMenu(
	//		LOCTEXT("SkeletonTreeBoneReductionForLOD_RemoveSelectedFromLOD", "Remove Selected..."),
	//		FText::GetEmpty(),
	//		FNewMenuDelegate::CreateStatic(&SSkeletonTree::CreateMenuForBoneReduction, this, LastCachedLODForPreviewMeshComponent, true)
	//	);

	//	MenuBuilder.AddSubMenu(
	//		LOCTEXT("SkeletonTreeBoneReductionForLOD_RemoveChildrenFromLOD", "Remove Children..."),
	//		FText::GetEmpty(),
	//		FNewMenuDelegate::CreateStatic(&SSkeletonTree::CreateMenuForBoneReduction, this, LastCachedLODForPreviewMeshComponent, false)
	//	);
	//}
	//MenuBuilder.EndSection();


//ISkeletonEditorModule& SkeletonEditorModule = FModuleManager::Get().LoadModuleChecked<ISkeletonEditorModule>("SkeletonEditor");
//auto& ToolbarExtenders = SkeletonEditorModule.GetAllSkeletonEditorToolbarExtenders();

//ToolbarExtenders.Add(ISkeletonEditorModule::FSkeletonEditorToolbarExtender::CreateRaw(this, &FMeshUtilities::GetSkeletonEditorToolbarExtender));
//SkeletonEditorExtenderHandle = ToolbarExtenders.Last().GetHandle();


//TSharedRef<FExtender> FMeshUtilities::GetSkeletonEditorToolbarExtender(const TSharedRef<FUICommandList> CommandList, TSharedRef<ISkeletonEditor> InSkeletonEditor)
//{
//	TSharedRef<FExtender> Extender = MakeShareable(new FExtender);

//	UMeshComponent* MeshComponent = InSkeletonEditor->GetPersonaToolkit()->GetPreviewMeshComponent();

//	Extender->AddToolBarExtension(
//		"Asset",
//		EExtensionHook::After,
//		CommandList,
//		FToolBarExtensionDelegate::CreateRaw(this, &FMeshUtilities::HandleAddSkeletalMeshActionExtenderToToolbar, MeshComponent)
//	);

//	return Extender;
//}


//void FMeshUtilities::HandleAddSkeletalMeshActionExtenderToToolbar(FToolBarBuilder& ParentToolbarBuilder, UMeshComponent* InMeshComponent)
//{
//	ParentToolbarBuilder.AddToolBarButton(
//		FUIAction(FExecuteAction::CreateLambda([this, InMeshComponent]()
//			{
//				ConvertMeshesToStaticMesh(TArray<UMeshComponent*>({ InMeshComponent }), InMeshComponent->GetComponentToWorld());
//			})),
//		NAME_None,
//				LOCTEXT("MakeStaticMesh", "Make Static Mesh"),
//				LOCTEXT("MakeStaticMeshTooltip", "Make a new static mesh out of the preview's current pose."),
//				FSlateIcon("EditorStyle", "Persona.ConvertToStaticMesh")
//				);
//}


}

void FCCSkeletalMeshTools::Unregister()
{
	FCCSkeletalMeshToolsCommands::Unregister();
	//UToolMenus::UnRegisterStartupCallback(this);
	//UToolMenus::UnregisterOwner(this);
	ISkeletalMeshEditorModule* SkelMeshEditorModule = FModuleManager::GetModulePtr<ISkeletalMeshEditorModule>("SkeletalMeshEditor");
	if (SkelMeshEditorModule)
	{
		TArray<ISkeletalMeshEditorModule::FSkeletalMeshEditorToolbarExtender>& Extenders = SkelMeshEditorModule->GetAllSkeletalMeshEditorToolbarExtenders();
		Extenders.RemoveAll([this](const ISkeletalMeshEditorModule::FSkeletalMeshEditorToolbarExtender& InDelegate) {return InDelegate.GetHandle() == SkelMeshEditorExtenderHandle; });
	}
}

void FCCSkeletalMeshTools::OpenMeshEditor(const TWeakPtr<ISkeletalMeshEditor>& Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
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
			else
			{
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
}

void FCCSkeletalMeshTools::OpenPoseMatcher(TWeakPtr<ISkeletalMeshEditor> Ed)
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
			else
			{
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
}

void FCCSkeletalMeshTools::CopyBonesLocation(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			TSharedRef<SCopyBonesLocationDialog> OptionsModal = SNew(SCopyBonesLocationDialog).CurrentMesh(Component);
			// this blocks until the modal is closed
			const FRefUpdatePoseOptions Options = OptionsModal->ShowModal();

			if (Options.UpdateType != ERefPoseOptionsReturnType::Cancel)
			{
				const TWeakObjectPtr<UObject> Asset = OptionsModal->GetSelectedAsset();
				check(Asset.IsValid());
				check(Asset.Get() != nullptr);
				{
					UCharacterCreationEditorLibrary::CopyBonesLocation(
						Component,
						Asset.Get()
					);
					Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
				}
			}
		}
	}
}

/*
void FCCSkeletalMeshTools::CopyBonesToPreview(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			TSharedRef<SCopyBonesLocationDialog> OptionsModal = SNew(SCopyBonesLocationDialog).CurrentMesh(Component);
			// this blocks until the modal is closed
			const FRefUpdatePoseOptions Options = OptionsModal->ShowModal();

			if (Options.UpdateType != ERefPoseOptionsReturnType::Cancel)
			{
				const TWeakObjectPtr<UObject> Asset = OptionsModal->GetSelectedAsset();
				check(Asset.IsValid());
				check(Asset.Get() != nullptr);
				{
					UCharacterCreationEditorLibrary::CopyBonesToPreview(
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
}
*/

void FCCSkeletalMeshTools::DeleteExtraBones(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			TSharedRef<SCopyBonesLocationDialog> OptionsModal = SNew(SCopyBonesLocationDialog).CurrentMesh(Component);
			// this blocks until the modal is closed
			const FRefUpdatePoseOptions Options = OptionsModal->ShowModal();

			if (Options.UpdateType != ERefPoseOptionsReturnType::Cancel)
			{
				const TWeakObjectPtr<UObject> Asset = OptionsModal->GetSelectedAsset();
				check(Asset.IsValid());
				check(Asset.Get() != nullptr);
				{
					UCharacterCreationEditorLibrary::DeleteExtraBones(
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
}

/*
void FCCSkeletalMeshTools::SaveCurrentReferencePose(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			UCharacterCreationEditorLibrary::SaveCurrentReferencePose(Component);
			Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
		}
	}
}
*/

void FCCSkeletalMeshTools::BakeCurrentGeometry(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			UCharacterCreationEditorLibrary::BakeCurrentGeometry(Component);
			Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
		}
	}
}

void FCCSkeletalMeshTools::ForceAssignSkeleton(TWeakPtr<ISkeletalMeshEditor> Ed)
{
#pragma region Checks
	//Run some checks
	if (!Ed.IsValid())
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletalMeshToolsNoValidEditor", "No valid SkeletalMeshEditor"));
		return;
	}

	UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent();
	if (!Component)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletalMeshToolsNoValidPreview", "No valid Preview Component"));
		return;
	}
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Component->GetSkinnedAsset());
	if (!SkeletalMesh)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletalMeshToolsNoValidSkeletalMesh", "No valid Preview SkeletalMesh"));
		return;
	}
	//End Run some checks
#pragma endregion Checks

	TSharedRef<SWindow> WidgetWindow = SNew(SWindow)
		.Title(LOCTEXT("ChooseSkeletonWindowTitle", "Choose Skeleton"))
		.ClientSize(FVector2D(400, 600));
	TSharedPtr<SSkeletonSelectorWindow> SkeletonSelectorWindow;
	WidgetWindow->SetContent
	(
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SAssignNew(SkeletonSelectorWindow, SSkeletonSelectorWindow)
			.Object(SkeletalMesh)
		.WidgetWindow(WidgetWindow)
		]
	);

	GEditor->EditorAddModalWindow(WidgetWindow);
	USkeleton* const SelectedSkeleton = SkeletonSelectorWindow->GetSelectedSkeleton();
	
	#pragma region Checks
//Run some extra checks
	if (!SelectedSkeleton)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletonMergeNoSkeletonSelected", "No valid Skeleton selected"));
		return;
	}
	if (SelectedSkeleton == SkeletalMesh->GetSkeleton())
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletonMergeSameSkeletonSelected", "The Skeleton selected is already assigned"));
		return;
	}
	//End Run some checks
#pragma endregion Checks

	FDlgMergeSkeleton AssetDlg(SkeletalMesh, SelectedSkeleton);
	if (AssetDlg.ShowModal() == FDlgMergeSkeleton::RM_Confirm)
	{
		bool const bIsSuccessful = UCharacterCreationEditorLibrary::MergeBonesToBoneTreeDeletingIntermediate(SelectedSkeleton, SkeletalMesh, AssetDlg.RequiredBones);
		Ed.Pin()->CloseWindow(EAssetEditorCloseReason::AssetEditorHostClosed);
		if (bIsSuccessful)
		{
			FAssetNotifications::SkeletonNeedsToBeSaved(SelectedSkeleton);
			//Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("SkeletonMergeBonesFailure", "Failed to merge bones to Skeleton"));
		}
	}
}

void FCCSkeletalMeshTools::ResetModifiedBones(TWeakPtr<ISkeletalMeshEditor> Ed)
{
	if (Ed.IsValid())
	{
		if (UDebugSkelMeshComponent* Component = Ed.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
		{
			UCharacterCreationEditorLibrary::ResetModifiedBones(Component);
			Ed.Pin()->GetPersonaToolkit()->GetEditableSkeleton()->RefreshBoneTree();
		}
	}
}

void FCCSkeletalMeshTools::PostUndo(bool bSuccess)
{

}

void FCCSkeletalMeshTools::PostRedo(bool bSuccess)
{

}

#undef LOCTEXT_NAMESPACE