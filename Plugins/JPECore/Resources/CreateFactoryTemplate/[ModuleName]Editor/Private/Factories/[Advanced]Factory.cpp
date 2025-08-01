// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/[Factory]Factory.h"
#include "ClassViewerModule.h"
#include "Modules/ModuleManager.h"
#include "Kismet2/SClassPickerDialog.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Model/[Factory].h"

#define LOCTEXT_NAMESPACE "U[Factory]"

U[Factory]Factory::U[Factory]Factory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = U[Factory]::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* U[Factory]Factory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if ([Factory]Class != nullptr)
	{
		return NewObject <U[Factory]>(InParent, [Factory]Class, InName, Flags | RF_Transactional);
	}
	else
	{
		check(InClass->IsChildOf(U[Factory]::StaticClass()));
		return NewObject <U[Factory]>(InParent, InClass, InName, Flags | RF_Transactional);
	}
}

bool U[Factory]Factory::ConfigureProperties()
{
	[Factory]Class = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<F[Factory]FilterViewer> Filter = MakeShareable <F[Factory]FilterViewer>(new F[Factory]FilterViewer);

#if ENGINE_MAJOR_VERSION == 4
	Options.ClassFilter = Filter;
#elif ENGINE_MAJOR_VERSION == 5
	Options.ClassFilters.Add(Filter.ToSharedRef());
#endif

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated;
	Filter->AllowedChildrenOfClasses.Add(U[Factory]::StaticClass());

	const FText TitleText = LOCTEXT("Create[Factory]Options", "Pick [Factory] Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, U[Factory]::StaticClass());

	if (bPressedOk)
	{
		[Factory]Class = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE