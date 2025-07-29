// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/ActionSequencesFactory.h"
#include "ActionSequences.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerModule.h"

#define LOCTEXT_NAMESPACE "ThePrison_UActionSequences"

UActionSequencesFactory::UActionSequencesFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UActionSequences::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UActionSequencesFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (ActionSequencesClass != nullptr)
	{
		return NewObject<UActionSequences>(InParent, ActionSequencesClass, Name, Flags | RF_Transactional);
	}
	check(Class->IsChildOf(UActionSequences::StaticClass()));
	return NewObject<UActionSequences>(InParent, Class, Name, Flags | RF_Transactional);
}

bool UActionSequencesFactory::ConfigureProperties()
{
	ActionSequencesClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FActionSequencesFilterViewer> Filter = MakeShareable<FActionSequencesFilterViewer>(new FActionSequencesFilterViewer);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(UActionSequences::StaticClass());

	const FText TitleText = LOCTEXT("CreateSequence", "Pick Sequence Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UActionSequences::StaticClass());

	if (bPressedOk)
	{
		ActionSequencesClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE
