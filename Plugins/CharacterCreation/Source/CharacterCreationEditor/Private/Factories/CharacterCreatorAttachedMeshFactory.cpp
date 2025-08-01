// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorAttachedMeshFactory.h"
#include "CharacterCreatorAttachedMesh.h"

//Import from CSV
#include "Misc/FileHelper.h"
#include "Serialization/Csv/CsvParser.h"
#include "Kismet/KismetStringLibrary.h"
#include "String/LexFromString.h"
#include "AssetRegistry/AssetRegistryModule.h"

UCharacterCreatorAttachedMeshFactory::UCharacterCreatorAttachedMeshFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorAttachedMesh::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

	bEditorImport = true;
	Formats.Add(TEXT("csvAttachedMeshs;Character creator AttachedMesh in CSV format"));
}

UObject* UCharacterCreatorAttachedMeshFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorAttachedMesh* NewAsset = NewObject<UCharacterCreatorAttachedMesh>(InParent, Class, Name, Flags);
	return NewAsset;
}
