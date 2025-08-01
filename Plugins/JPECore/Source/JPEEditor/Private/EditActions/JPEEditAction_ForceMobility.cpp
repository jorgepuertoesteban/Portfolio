// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/JPEEditAction_ForceMobility.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Dialogs/Dialogs.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"

#include "Widgets/Input/SEditableText.h"
#include "Editor.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Styling/ISlateStyle.h"
#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "JPEEditAction_ForceMobility"
#endif


void FJPEEditAction_ForceMobility::ExecuteAction()
{
#if ENGINE_MAJOR_VERSION == 5
	USelection* SelectedActors = GEditor->GetSelectedActors();

	EComponentMobility::Type Value;

	TSharedRef<SSegmentedControl<EComponentMobility::Type>> ButtonOptionsPanel =
		SNew(SSegmentedControl<EComponentMobility::Type>)
		.Value(EComponentMobility::Static)
		.OnValueChanged_Lambda(
			[&Value](EComponentMobility::Type NewValue)
	{
		Value = NewValue;
	}
	);

	FText StaticTooltip = LOCTEXT("Mobility_Static_Tooltip", "A static object can't be changed in game.\n* Allows Baked Lighting\n* Fastest Rendering");

	// Static Mobility
	ButtonOptionsPanel->AddSlot(EComponentMobility::Static)
		.HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 3.0f, 0.0f)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("PropertyWindow.MobilityStatic"))
		.ColorAndOpacity(FSlateColor::UseForeground())
		]
	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Font(FAppStyle::Get().GetFontStyle("PropertyWindow.MobilityFont"))
		.Text(LOCTEXT("Static", "Static"))
		]
		]
	.ToolTip(StaticTooltip);

	FText StationaryTooltip = LOCTEXT("Mobility_Stationary_Object_Tooltip", "A stationary object can be changed in game but not moved, and enables cached lighting methods. \n* Cached Dynamic Shadows.");

	ButtonOptionsPanel->AddSlot(EComponentMobility::Stationary)
		.HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 3.0f, 0.0f)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("PropertyWindow.MobilityStationary"))
		.ColorAndOpacity(FSlateColor::UseForeground())
		]
	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Font(FAppStyle::Get().GetFontStyle("PropertyWindow.MobilityFont"))
		.Text(LOCTEXT("Stationary", "Stationary"))
		]
		]
	.ToolTip(StationaryTooltip);

	FText MovableTooltip = LOCTEXT("Mobility_Movable_Tooltip", "Movable objects can be moved and changed in game.\n* Totally Dynamic\n* Casts a Dynamic Shadow \n* Slowest Rendering");

	// Movable Mobility
	ButtonOptionsPanel->AddSlot(EComponentMobility::Movable)
		.HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 3.0f, 0.0f)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("PropertyWindow.MobilityMoveable"))
		.ColorAndOpacity(FSlateColor::UseForeground())
		]
	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Font(FAppStyle::Get().GetFontStyle("PropertyWindow.MobilityFont"))
		.Text(LOCTEXT("Movable", "Movable"))
		]
		]
	.ToolTip(MovableTooltip);

	ButtonOptionsPanel->RebuildChildren();
	//***************************************************************************

	SGenericDialogWidget::OpenDialog(
		LOCTEXT("FJPEEditAction_RenameSelectedActors", "Force Mobility of Selected Actors"),
		ButtonOptionsPanel,
		SGenericDialogWidget::FArguments(),
		true
	);

	//uint8 MobilityByte = SegmentedControl->GetValue();

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* Actor = Cast<AActor>(*Iter))
		{
			for (UActorComponent* Comp : Actor->GetComponents())
			{
				if (USceneComponent* SceneComponent = Cast<USceneComponent>(Comp))
				{
					SceneComponent->SetMobility(Value);
				}
			}
		}
	}
#endif
}

#undef LOCTEXT_NAMESPACE