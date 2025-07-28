// Created by Bionic Ape. All Rights Reserved.

#include "Factories/ActionMontagesFactory.h"
#include "ActionMontages.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ClassViewerModule.h"

#define LOCTEXT_NAMESPACE "ThePrison_UActionMontages"

UActionMontagesFactory::UActionMontagesFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UActionMontages::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UActionMontagesFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (ActionMontagesClass != nullptr)
	{
		return NewObject<UActionMontages>(InParent, ActionMontagesClass, Name, Flags | RF_Transactional);
	}
	check(Class->IsChildOf(UActionMontages::StaticClass()));
	return NewObject<UActionMontages>(InParent, Class, Name, Flags | RF_Transactional);
}

bool UActionMontagesFactory::ConfigureProperties()
{
	ActionMontagesClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FActionMontagesFilterViewer> Filter = MakeShareable<FActionMontagesFilterViewer>(new FActionMontagesFilterViewer);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(UActionMontages::StaticClass());

	const FText TitleText = LOCTEXT("CreateSequence", "Pick Sequence Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UActionMontages::StaticClass());

	if (bPressedOk)
	{
		ActionMontagesClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE
