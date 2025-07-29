// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/ActionTypeFactory.h"
#include "ClassViewerModule.h"
#include "Modules/ModuleManager.h"
#include "Kismet2/SClassPickerDialog.h"
#include "ActionType.h"

#define LOCTEXT_NAMESPACE "UActionType"

UActionTypeFactory::UActionTypeFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UActionType::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UActionTypeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (ActionTypeClass != nullptr)
	{
		return NewObject <UActionType>(InParent, ActionTypeClass, InName, Flags | RF_Transactional);
	}
	else
	{
		check(InClass->IsChildOf(UActionType::StaticClass()));
		return NewObject <UActionType>(InParent, InClass, InName, Flags | RF_Transactional);
	}
}

bool UActionTypeFactory::ConfigureProperties()
{
	ActionTypeClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FActionTypeFilterViewer> Filter = MakeShareable <FActionTypeFilterViewer>(new FActionTypeFilterViewer);
	Options.ClassFilters.Add(Filter.ToSharedRef());

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(UActionType::StaticClass());

	const FText TitleText = LOCTEXT("CreateActionTypeOptions", "Pick ActionType Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UActionType::StaticClass());

	if (bPressedOk)
	{
		ActionTypeClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE