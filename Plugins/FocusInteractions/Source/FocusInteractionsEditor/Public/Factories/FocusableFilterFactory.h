// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ClassViewerFilter.h"
#include "UObject/Class.h"
#include "FocusableFilterFactory.generated.h"

class IClassViewerFilter;
class UFocusableFilter;

/**
 *
 */
UCLASS()
class FOCUSINTERACTIONSEDITOR_API UFocusableFilterFactory : public UFactory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FocusableFilter")
	TSubclassOf<UFocusableFilter> FocusableFilterClass;

public:
	UFocusableFilterFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	bool ConfigureProperties() override;
	virtual bool CanCreateNew() const override { return true; }
	// End of UFactory interface

};

class FFocusableFilterFilterViewer : public IClassViewerFilter
{
public:
	TSet<const UClass*> AllowedChildrenOfClasses;
	EClassFlags DisallowedClassFlags;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< class FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags) && InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags) && InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};