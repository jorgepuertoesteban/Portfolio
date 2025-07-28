// Created by Bionic Ape. All Rights Reserved.

#include "Factories/ActionsSetFactory.h"
#include "ClassViewerModule.h"
#include "Modules/ModuleManager.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ActionsSet.h"

#define LOCTEXT_NAMESPACE "UActionsSet"

UActionsSetFactory::UActionsSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UActionsSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UActionsSetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (ActionsSetClass != nullptr)
	{
		return NewObject <UActionsSet>(InParent, ActionsSetClass, InName, Flags | RF_Transactional);
	}
	else
	{
		check(InClass->IsChildOf(UActionsSet::StaticClass()));
		return NewObject <UActionsSet>(InParent, InClass, InName, Flags | RF_Transactional);
	}
}

bool UActionsSetFactory::ConfigureProperties()
{
	ActionsSetClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FActionsSetFilterViewer> Filter = MakeShareable <FActionsSetFilterViewer>(new FActionsSetFilterViewer);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(UActionsSet::StaticClass());

	const FText TitleText = LOCTEXT("CreateActionsSetOptions", "Pick ActionsSet Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UActionsSet::StaticClass());

	if (bPressedOk)
	{
		ActionsSetClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE