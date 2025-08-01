// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/JPEAssetFactory.h"

#include "Utils/AssetUtils.h"

UJPEAssetFactory::UJPEAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UObject::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
	bEditorImport = true;
	Formats.Add(TEXT("jpeassets;JPEAssets in JSON format"));
}

UObject* UJPEAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                           UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags);
}

UObject* UJPEAssetFactory::ImportObject(UClass* InClass, UObject* InOuter, FName InName, EObjectFlags InFlags,
                                       const FString& Filename, const TCHAR* Parms, bool& OutCanceled)
{
	UE_LOG(LogTemp, Log,
	       TEXT("UJPEAssetFactory::ImportObject: Trying to import JPEAssets from .jpeassets extension with JSON format."));

	AdditionalImportedObjects.Empty();
	CurrentFilename = Filename;

	TArray<UObject*> ImportedAssets = UAssetUtils::LoadFromJsonFile(Filename, this);

	if (ImportedAssets.Num() <= 0)
	{
		return nullptr;
	}
	
	for (int i = 1; i < ImportedAssets.Num(); ++i)
	{
		AdditionalImportedObjects.Add(ImportedAssets[i]);
	}

	CurrentFilename = TEXT("");

	return ImportedAssets[0];
}
