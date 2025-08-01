// Created by Jorge Puerto. All Rights Reserved.

#include "Workbench/SCCWorkbench.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Workbench/SCCWorkbenchViewport.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Workbench/SCCWorkbenchViewport.h"
#include "Templates/SharedPointer.h"


#define LOCTEXT_NAMESPACE "SCCWorkbench"

void SCCWorkbench::Construct(const FArguments& InArgs)
{

	//SCompoundWidget::Construct(SCompoundWidget::FArguments());

	static const FSlateFontInfo BannerFont = FCoreStyle::GetDefaultFontStyle("Bold", 16);

	ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f))
		//[
		//	/*SAssignNew(Categories, SLaunchPadCategories)
		//	.OnSelectionChanged(this, &SLaunchPad::OnCategorySelectionChanged)*/
		//]
		]
	+ SOverlay::Slot()
		//[
		//	CreateSidebarLogo().ToSharedRef()
		//]
		]
	+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.Padding(5)
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.75f, 0.75f, 0.75f))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("LaunchPadBannerText", "Character Creation - Launch Pad"))
		.Font(BannerFont)
		.Justification(ETextJustify::Center)
		.ShadowOffset(FVector2D(1, 2))
		.ShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.75f))
		]
		]

	+ SVerticalBox::Slot()
		.Padding(6, 0, 0, 6)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		//[
		//[
		//	SAssignNew(BreadCrumb, SLaunchPadBreadCrumb)
		//	.OnNavigation(this, &SLaunchPad::OnBreadCrumbNavigation)
		//]
		+SHorizontalBox::Slot()
		[
			SNullWidget::NullWidget
		]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		[
			SAssignNew(PreviewViewport, SCCWorkbenchViewport)
		]
		]
		]
		]
		];
}

#undef LOCTEXT_NAMESPACE