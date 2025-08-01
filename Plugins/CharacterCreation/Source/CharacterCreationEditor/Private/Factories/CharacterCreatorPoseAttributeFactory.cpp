// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorPoseAttributeFactory.h"
#include "CharacterCreatorPoseAttribute.h"
#include "Engine/SkeletalMesh.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SSeparator.h"
#include "Editor.h"

#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "CharacterCreatorPoseAttributeFactory"

DECLARE_DELEGATE_FourParams(FOnPoseConfigureUserAction, bool, USkeletalMesh*, USkeletalMesh*, USkeletalMesh*);

class SPoseConfigureWindow : public SWindow
{
public:
	SLATE_BEGIN_ARGS(SPoseConfigureWindow)
		{}

		SLATE_ARGUMENT(USkeletalMesh*, SourceBaseSkeletalMesh)
		SLATE_ARGUMENT(USkeletalMesh*, SourceMinValueSkeletalMesh)
		SLATE_ARGUMENT(USkeletalMesh*, SourceMaxValueSkeletalMesh)
		SLATE_ARGUMENT(FOnPoseConfigureUserAction, UserActionHandler)
		SLATE_ARGUMENT(FSimpleDelegate, OnCreateCanceled)

	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs)
	{
		UserActionHandler = InArgs._UserActionHandler;
		SourceBaseSkeletalMesh = InArgs._SourceBaseSkeletalMesh;
		SourceMinValueSkeletalMesh = InArgs._SourceMinValueSkeletalMesh;
		SourceMaxValueSkeletalMesh = InArgs._SourceMaxValueSkeletalMesh;

		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig BaseAssetPickerConfig;
		/** The asset picker will only show sequences */
		BaseAssetPickerConfig.SelectionMode = ESelectionMode::Single;
		BaseAssetPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		/** The delegate that fires when an asset was selected */
		BaseAssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SPoseConfigureWindow::OnSourceBaseSkeletalMeshSelected);
		if (SourceBaseSkeletalMesh != nullptr)
		{
			BaseAssetPickerConfig.InitialAssetSelection = FAssetData(SourceBaseSkeletalMesh);
		}

		/** The default view mode should be a list view */
		BaseAssetPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		BaseAssetPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;

		FAssetPickerConfig MinValueAssetPickerConfig;
		MinValueAssetPickerConfig.SelectionMode = ESelectionMode::Single;
		/** The asset picker will only show sequences */
		MinValueAssetPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		/** The delegate that fires when an asset was selected */
		MinValueAssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SPoseConfigureWindow::OnSourceMinValueSkeletalMeshSelected);
		if (SourceMinValueSkeletalMesh != nullptr)
		{
			MinValueAssetPickerConfig.InitialAssetSelection = FAssetData(SourceMinValueSkeletalMesh);
		}

		/** The default view mode should be a list view */
		MinValueAssetPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		MinValueAssetPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;

		FAssetPickerConfig MaxValueAssetPickerConfig;
		MaxValueAssetPickerConfig.SelectionMode = ESelectionMode::Single;
		/** The asset picker will only show sequences */
		MaxValueAssetPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		/** The delegate that fires when an asset was selected */
		MaxValueAssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SPoseConfigureWindow::OnSourceMaxValueSkeletalMeshSelected);
		if (SourceMaxValueSkeletalMesh != nullptr)
		{
			MaxValueAssetPickerConfig.InitialAssetSelection = FAssetData(SourceMaxValueSkeletalMesh);
		}

		/** The default view mode should be a list view */
		MaxValueAssetPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		MaxValueAssetPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;

		SWindow::Construct(SWindow::FArguments()
			.Title(LOCTEXT("CreatePoseAttributeAssetOptions", "Create Pose Attribute Asset"))
			.SizingRule(ESizingRule::UserSized)
			.ClientSize(FVector2D(1500, 1500 * 9 / 16))
			.SupportsMinimize(false)
			.SupportsMaximize(false)
			[
				SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Menu.Background"))
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Base Skeletal Mesh ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(BaseAssetPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Minimum Value Skeletal Mesh ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(MinValueAssetPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Maximum Value Skeletal Mesh ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(MaxValueAssetPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]

							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SUniformGridPanel)
									.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
									.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
									.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
									+ SUniformGridPanel::Slot(0, 0)
									[
										SNew(SButton)
											.Text(LOCTEXT("Accept", "Accept"))
											.HAlign(HAlign_Center)
											.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
											.IsEnabled(this, &SPoseConfigureWindow::CanAccept)
											.OnClicked_Raw(this, &SPoseConfigureWindow::OnAccept)
									]
									+ SUniformGridPanel::Slot(1, 0)
									[
										SNew(SButton)
											.Text(LOCTEXT("Cancel", "Cancel"))
											.HAlign(HAlign_Center)
											.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
											.OnClicked_Raw(this, &SPoseConfigureWindow::OnCancel)
									]

							]
					]
			]);
	}

	bool CanAccept() const
	{
		return SourceBaseSkeletalMesh != nullptr && SourceBaseSkeletalMesh->GetSkeleton() && SourceMinValueSkeletalMesh != nullptr && SourceMinValueSkeletalMesh->GetSkeleton() && SourceMaxValueSkeletalMesh != nullptr && SourceMaxValueSkeletalMesh->GetSkeleton() && UserActionHandler.IsBound();
	}

	FReply OnAccept()
	{
		if (CanAccept())
		{
			UserActionHandler.Execute(true, SourceBaseSkeletalMesh, SourceMinValueSkeletalMesh, SourceMaxValueSkeletalMesh);
		}

		RequestDestroyWindow();
		return FReply::Handled();
	}

	FReply OnCancel()
	{
		if (UserActionHandler.IsBound())
		{
			UserActionHandler.Execute(false, nullptr, nullptr, nullptr);
		}

		RequestDestroyWindow();
		return FReply::Handled();
	}

	void OnSourceBaseSkeletalMeshSelected(const FAssetData& SelectedAsset)
	{
		SourceBaseSkeletalMesh = Cast<USkeletalMesh>(SelectedAsset.GetAsset());
	}

	void OnSourceMinValueSkeletalMeshSelected(const FAssetData& SelectedAsset)
	{
		SourceMinValueSkeletalMesh = Cast<USkeletalMesh>(SelectedAsset.GetAsset());
	}

	void OnSourceMaxValueSkeletalMeshSelected(const FAssetData& SelectedAsset)
	{
		SourceMaxValueSkeletalMesh = Cast<USkeletalMesh>(SelectedAsset.GetAsset());
	}

private:
	USkeletalMesh* SourceBaseSkeletalMesh;
	USkeletalMesh* SourceMinValueSkeletalMesh;
	USkeletalMesh* SourceMaxValueSkeletalMesh;
	FOnPoseConfigureUserAction	UserActionHandler;
};


UCharacterCreatorPoseAttributeFactory::UCharacterCreatorPoseAttributeFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorPoseAttribute::StaticClass();
	bCreateNew = true;
}

UObject* UCharacterCreatorPoseAttributeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {

	if (BaseSkeletalMesh == nullptr || MinValueSkeletalMesh == nullptr || MaxValueSkeletalMesh == nullptr)
	{
		return nullptr;
	}
	if (BaseSkeletalMesh->GetSkeleton() != MinValueSkeletalMesh->GetSkeleton() || BaseSkeletalMesh->GetSkeleton() != MaxValueSkeletalMesh->GetSkeleton())
	{
		return nullptr;
	}
	FReferenceSkeleton BaseSkeleton = BaseSkeletalMesh->GetRefSkeleton();
	FReferenceSkeleton MinValueSkeleton = MinValueSkeletalMesh->GetRefSkeleton();
	FReferenceSkeleton MaxValueSkeleton = MaxValueSkeletalMesh->GetRefSkeleton();

	TArray<FTransform> BaseTransforms = BaseSkeleton.GetRefBonePose();
	TArray<FTransform> MinValueTransforms = MinValueSkeleton.GetRefBonePose();
	TArray<FTransform> MaxValueTransforms = MaxValueSkeleton.GetRefBonePose();

	UCharacterCreatorPoseAttribute* NewAsset = NewObject<UCharacterCreatorPoseAttribute>(InParent, Class, Name, Flags);

	for (int32 Index = 0; Index < BaseTransforms.Num(); Index++)
	{
		if (!UKismetMathLibrary::NearlyEqual_TransformTransform(BaseTransforms[Index], MinValueTransforms[Index], .1f, .1f, .1f))
		{
			FTransform Offset = FTransform(MinValueTransforms[Index].GetRotation() - BaseTransforms[Index].GetRotation(), MinValueTransforms[Index].GetLocation() - BaseTransforms[Index].GetLocation(), MinValueTransforms[Index].GetScale3D() - BaseTransforms[Index].GetScale3D());
			NewAsset->MinPoseOffsets.Add(BaseSkeleton.GetBoneName(Index), Offset);
		}
		if (!UKismetMathLibrary::NearlyEqual_TransformTransform(BaseTransforms[Index], MaxValueTransforms[Index], .1f, .1f, .1f))
		{
			FTransform Offset = FTransform(MaxValueTransforms[Index].GetRotation() - BaseTransforms[Index].GetRotation(), MaxValueTransforms[Index].GetLocation() - BaseTransforms[Index].GetLocation(), MaxValueTransforms[Index].GetScale3D() - BaseTransforms[Index].GetScale3D());
			NewAsset->MaxPoseOffsets.Add(BaseSkeleton.GetBoneName(Index), Offset);
		}
	}

	return NewAsset;
}

bool UCharacterCreatorPoseAttributeFactory::ConfigureProperties()
{
	TSharedPtr<SWindow> PickerWindow = SNew(SPoseConfigureWindow)
		.SourceBaseSkeletalMesh(BaseSkeletalMesh)
		.SourceMinValueSkeletalMesh(MinValueSkeletalMesh)
		.SourceMaxValueSkeletalMesh(MaxValueSkeletalMesh)
		.UserActionHandler(FOnPoseConfigureUserAction::CreateUObject(this, &UCharacterCreatorPoseAttributeFactory::OnWindowUserActionDelegate));

	BaseSkeletalMesh = nullptr;
	MinValueSkeletalMesh = nullptr;
	MaxValueSkeletalMesh = nullptr;

	GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
	PickerWindow.Reset();

	return BaseSkeletalMesh != nullptr && MinValueSkeletalMesh != nullptr && MaxValueSkeletalMesh != nullptr;
}

void UCharacterCreatorPoseAttributeFactory::OnWindowUserActionDelegate(bool bCreate, USkeletalMesh* InBaseSkeletalMesh, USkeletalMesh* InMinValueSkeletalMesh, USkeletalMesh* InMaxValueSkeletalMesh)
{
	BaseSkeletalMesh = InBaseSkeletalMesh;
	MinValueSkeletalMesh = InMinValueSkeletalMesh;
	MaxValueSkeletalMesh = InMaxValueSkeletalMesh;
}

#undef LOCTEXT_NAMESPACE
