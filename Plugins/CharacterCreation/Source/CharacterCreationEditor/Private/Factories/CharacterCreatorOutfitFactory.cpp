// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorOutfitFactory.h"
#include "CharacterCreatorOutfit.h"

//Import from CSV
#include "Misc/FileHelper.h"
#include "Serialization/Csv/CsvParser.h"
#include "Kismet/KismetStringLibrary.h"
#include "String/LexFromString.h"
#include "AssetRegistry/AssetRegistryModule.h"

static FString IdField = "DatabaseId";
static FString NameField = "OutfitName";
static FString ShouldImportField = "bShouldImport";

UCharacterCreatorOutfitFactory::UCharacterCreatorOutfitFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorOutfit::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

	bEditorImport = true;
	Formats.Add(TEXT("csvoutfits;Character creator outfit in CSV format"));
}

UObject* UCharacterCreatorOutfitFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorOutfit* NewAsset = NewObject<UCharacterCreatorOutfit>(InParent, Class, Name, Flags);
	return NewAsset;
}

UObject* UCharacterCreatorOutfitFactory::ImportObject(UClass* InClass, UObject* InOuter, FName InName, EObjectFlags Flags, const FString& FilePath, const TCHAR* Parms, bool& OutCanceled)
{
	UE_LOG(LogTemp, Log, TEXT("UCharacterCreatorOutfitFactory::ImportObject: Trying to import Outfit from .csvoutfits extension with JSON format"));

	AdditionalImportedObjects.Empty();
	CurrentFilename = FilePath;


	// sanity check the file size of the impending import and prompt
	// the user if they wish to continue if the file size is very large
	const int64 FileSize = IFileManager::Get().FileSize(*CurrentFilename);

	// Hack do not hash files that are huge.  They take forever anyway
	const int32 Gigabyte = 1024 * 1024 * 1024;
	if (FileSize < Gigabyte)
	{
		FileHash = FMD5Hash::HashFile(*CurrentFilename);
	}


	//bool bIsSuccesful = UInventorySystemUtils::LoadFromOutfitsCSVFile(InClass, Filename, Outfits, InOuter, (uint8) Flags, InName);
	//////////////////////////////////////////////////////////
	//bool UInventorySystemUtils::LoadFromOutfitsCSVFile(const FString & FilePath, TArray<UInventoryOutfit*>&Outfits, UObject * Outer, uint8 InFlags)


	UPackage* RootPackage = Cast<UPackage>(InOuter);
	if (!RootPackage)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to import CSV table from '%s'. Outer is not a UPackage object."), *FilePath);
		return nullptr;
	}

	FString const OutfitsFolderName = RootPackage->GetName() + "/";

	FString FileString;
	bool const bIsFileLoaded = FFileHelper::LoadFileToString(FileString, *FilePath);
	if (!bIsFileLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to import CSV table from '%s'. LoadFileToString failed."), *FilePath);
		return nullptr;
	}

	const FCsvParser Parser(FileString);
	const FCsvParser::FRows& Rows = Parser.GetRows();

	if (Rows.Num() <= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to import CSV table from '%s'. Incorrect number of rows (must be at least 2)."), *FilePath);
		return nullptr;
	}

	//Definitions
	struct FPropertyHelper
	{
		FProperty* Property;
		int32 ColumnIndex = INDEX_NONE;

		bool IsValid()
		{
			return Property && ColumnIndex != INDEX_NONE;
		}
	};
	//End Definitions

#pragma region CreatePropertiesMap
	//Map Properties to columns
	TMap<FString, FPropertyHelper> PropertiesMap;
	{
		for (TFieldIterator<FProperty> PropIt(InClass); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			UE_LOG(LogTemp, Log, TEXT("Property in Class: '%s'."), *Property->GetFName().ToString());
			FString Name;
			Property->GetName(Name);
			FPropertyHelper& PropertyHelper = PropertiesMap.FindOrAdd(Name);
			PropertyHelper.Property = Property;
		}

		const TArray<const TCHAR*>& HeaderCells = Rows[0];

		for (int32 CellIdx = 0; CellIdx < HeaderCells.Num(); ++CellIdx)
		{
			const TCHAR* HeaderCell = HeaderCells[CellIdx];
			if (HeaderCell)
			{
				UE_LOG(LogTemp, Log, TEXT("Column found: '%s'."), HeaderCell);
				FPropertyHelper& PropertyHelper = PropertiesMap.FindOrAdd(HeaderCell);
				PropertyHelper.ColumnIndex = CellIdx;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Empty Cell in column index: '%s'."), *FString::FromInt(CellIdx));
			}
		}
	}
#pragma endregion CreatePropertiesMap

	if (!PropertiesMap.Contains(IdField) && PropertiesMap[IdField].IsValid())
	{
		//There must be a name field
		UE_LOG(LogTemp, Error, TEXT("Failed to import CSV table from '%s'. It is required to have a field named [%s]."), *FilePath, *IdField);
		return nullptr;
	}

	TArray<UCharacterCreatorOutfit*> Outfits;
	// Import rows
	{
		for (int32 RowIdx = 1; RowIdx < Rows.Num(); ++RowIdx)
		{
			const TArray<const TCHAR*>& Cells = Rows[RowIdx];

			const FString& OutfitId = Cells[PropertiesMap[IdField].ColumnIndex];

			//Check ID Value
			if (OutfitId.IsEmpty())
			{
				UE_LOG(LogTemp, Log, TEXT("The Outfit at the row %s does not have required column %s"), *UKismetStringLibrary::Conv_IntToString(RowIdx), *IdField);
				continue;
			}

			const FString& OutfitName = Cells[PropertiesMap[NameField].ColumnIndex];

			//Check Name Value
			if (OutfitName.IsEmpty())
			{
				UE_LOG(LogTemp, Log, TEXT("The Outfit at the row %s does not have required column %s"), *UKismetStringLibrary::Conv_IntToString(RowIdx), *NameField);
				continue;
			}

			//bShouldImport?
			if (PropertiesMap.Contains(ShouldImportField))
			{
				//There's a column indicating if it should be imported
				const FString& ShouldBeImported = Cells[PropertiesMap[ShouldImportField].ColumnIndex];
				if (!ShouldBeImported.ToBool())
				{
					UE_LOG(LogTemp, Log, TEXT("The Outfit %s is marked as not to be imported."), *OutfitId);
					continue;
				}
			}
			//End bShouldImport

			FString const FileName = "O_" + OutfitName + "_" + OutfitId;
			FString const PackageName = OutfitsFolderName + FileName;

			UPackage* Package = CreatePackage(*PackageName);
			if (!Package)
			{
				UE_LOG(LogTemp, Error, TEXT("The Package wasn't created: %s"), *PackageName);
				continue;
			}

			//////////////////////////////////////////////////////////////////////////
			// Create the Outfit
			//////////////////////////////////////////////////////////////////////////
			UCharacterCreatorOutfit* Outfit = NewObject<UCharacterCreatorOutfit>(Package, InClass, FName(FileName), (EObjectFlags)Flags);
			


			//Set Mesh Array
			//Outfit->Meshes.SetNum(3); //This way we ensure that the positions on the array match the enum ids

			const FString& OutfitNrwMeshId = Cells[PropertiesMap["Meshes:nrw"].ColumnIndex];
			USkeletalMesh* NRWMesh = LoadObject<USkeletalMesh>(nullptr, *OutfitNrwMeshId);
			Outfit->Meshes.Add(NRWMesh);
			//Outfit->Meshes[0] = NRWMesh;

			const FString& OutfitOvwMeshId = Cells[PropertiesMap["Meshes:ovw"].ColumnIndex];
			USkeletalMesh* OVWMesh = LoadObject<USkeletalMesh>(nullptr, *OutfitOvwMeshId);
			Outfit->Meshes.Add(OVWMesh);
			//Outfit->Meshes[1] = OVWMesh;

			const FString& OutfitUnwMeshId = Cells[PropertiesMap["Meshes:unw"].ColumnIndex];
			USkeletalMesh* UNWMesh = LoadObject<USkeletalMesh>(nullptr, *OutfitUnwMeshId);
			Outfit->Meshes.Add(UNWMesh);
			//Outfit->Meshes[2] = UNWMesh;
			//End Set Mesh Array
			
			
			//Set FProperty values
			for (auto It = PropertiesMap.CreateConstIterator(); It; ++It)
			{
				const FString& KeyName = It.Key();
				FPropertyHelper PropertyHelper = It.Value();

				UE_LOG(LogTemp, Log, TEXT("FPropertyHelper: Keyname: '%s' | PropertyHelper.Property %s | PropertyHelper.ColumnIndex: %s"),
					*KeyName,
					PropertyHelper.Property ? *PropertyHelper.Property->GetName() : TEXT("Null"),
					*FString::FromInt(PropertyHelper.ColumnIndex));

				//Validation
				if (!PropertyHelper.IsValid() || !Cells.IsValidIndex(PropertyHelper.ColumnIndex))
				{
					continue;
				}

				const FString& CellValue = Cells[PropertyHelper.ColumnIndex];

				if (CellValue.IsEmpty())
				{
					continue;
				}
				//End Validation

				//FObjectProperty
				if (const FObjectProperty* ObjectProperty = CastField<const FObjectProperty>(PropertyHelper.Property))
				{
					//get reference
					if (UObject* Object = LoadObject<UObject>(nullptr, *CellValue))
					{
						if (UObject** ValuePtr = ObjectProperty->ContainerPtrToValuePtr<UObject*>(Outfit))
						{
							*ValuePtr = Object;
						}
					}
				}
				//FStrProperty
				else if (const FStrProperty* StrProperty = CastField<const FStrProperty>(PropertyHelper.Property))
				{
					if (FString* ValuePtr = StrProperty->ContainerPtrToValuePtr<FString>(Outfit))
					{
						*ValuePtr = CellValue;
					}
				}
				//FNameProperty
				else if (const FNameProperty* NameProperty = CastField<const FNameProperty>(PropertyHelper.Property))
				{
					if (FName* ValuePtr = NameProperty->ContainerPtrToValuePtr<FName>(Outfit))
					{
						*ValuePtr = FName(*CellValue);
					}
				}
				//FIntProperty
				else if (const FIntProperty* IntProperty = CastField<const FIntProperty>(PropertyHelper.Property))
				{
					if (int32* ValuePtr = IntProperty->ContainerPtrToValuePtr<int32>(Outfit))
					{
						*ValuePtr = UKismetStringLibrary::Conv_StringToInt(CellValue);
					}
				}
				//FFloatProperty
				else if (const FFloatProperty* FloatProperty = CastField<const FFloatProperty>(PropertyHelper.Property))
				{
					if (float* ValuePtr = FloatProperty->ContainerPtrToValuePtr<float>(Outfit))
					{
						//*ValuePtr = UKismetStringLibrary::Conv_StringToFloat(CellValue);//This was deprecated
						LexFromString(*ValuePtr, CellValue);//TODO: check if this method that replace UKismetStringLibrary::Conv_StringToFloat works correctly
					}
				}
				//FBoolProperty
				else if (const FBoolProperty* BoolProperty = CastField<const FBoolProperty>(PropertyHelper.Property))
				{
					if (bool* ValuePtr = BoolProperty->ContainerPtrToValuePtr<bool>(Outfit))
					{
						*ValuePtr = CellValue.ToBool();
					}
				}
				//number (in this setup, only used for Enums)
				else if (const FNumericProperty* NumericProperty = CastField<const FNumericProperty>(PropertyHelper.Property))
				{
					if (NumericProperty->IsEnum())
					{
						if (uint8* ValuePtr = NumericProperty->ContainerPtrToValuePtr<uint8>(Outfit))
						{
							// see if we were passed a string for the enum
							const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
							check(Enum); // should be assured by IsEnum()
							int64 IntValue = Enum->GetValueByName(FName(*CellValue));
							if (IntValue == INDEX_NONE)
							{
								UE_LOG(LogTemp, Error, TEXT("Unable import enum %s from string value %s for property %s"), *Enum->CppType, *CellValue, *PropertyHelper.Property->GetNameCPP());
								return nullptr;
							}
							*ValuePtr = IntValue;
						}
					}
				}
				//NO MATCHING PROPERTY
				else
				{
					UE_LOG(LogTemp, Log, TEXT("No Matching Property with the compatible values: '%s'"), *KeyName);
				}
			}
			//End FProperty values

			//Mark as asset created
			Outfits.Add(Outfit);
			FAssetRegistryModule::AssetCreated(Outfit);
			Outfit->MarkPackageDirty();
			Outfit->PostEditChange();
			AdditionalImportedObjects.Add(Outfit);
			//End Mark as asset created
		}
	}
	//////////////////////////////////////////////////////////////////////////

	CurrentFilename = TEXT("");

	return Outfits[0];
}