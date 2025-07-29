// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/FocusableFilterFactory.h"
#include "ClassViewerModule.h"
#include "Modules/ModuleManager.h"
#include "Kismet2/SClassPickerDialog.h"
#include "FocusableFilter.h"

#define LOCTEXT_NAMESPACE "UFocusableFilter"

UFocusableFilterFactory::UFocusableFilterFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UFocusableFilter::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UFocusableFilterFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (FocusableFilterClass != nullptr)
	{
		return NewObject <UFocusableFilter>(InParent, FocusableFilterClass, InName, Flags | RF_Transactional);
	}
	else
	{
		check(InClass->IsChildOf(UFocusableFilter::StaticClass()));
		return NewObject <UFocusableFilter>(InParent, InClass, InName, Flags | RF_Transactional);
	}
}

bool UFocusableFilterFactory::ConfigureProperties()
{
	FocusableFilterClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FFocusableFilterFilterViewer> Filter = MakeShareable <FFocusableFilterFilterViewer>(new FFocusableFilterFilterViewer);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(UFocusableFilter::StaticClass());

	const FText TitleText = LOCTEXT("CreateFocusableFilterOptions", "Pick FocusableFilter Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UFocusableFilter::StaticClass());

	if (bPressedOk)
	{
		FocusableFilterClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE