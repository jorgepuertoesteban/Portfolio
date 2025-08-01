// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreationJsonUtils.h"

#include "CharacterCreationTypes.h"
#include "CharacterCreator.h"
#include "CharacterCreatorAttribute.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreatorMatAttribute.h"
#include "CharacterCreatorModel.h"
#include "CharacterCreatorOutfit.h"
#include "CharacterCreatorOutfitSlot.h"
#include "CharacterCreatorPoseAttribute.h"
#include "CharacterCreatorVectorMatAttribute.h"
#include "Logs/CharacterCreationLog.h"
#include "Misc/FileHelper.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/UObjectGlobals.h"

UCharacterCreator* UCharacterCreationJsonUtils::LoadFromJsonFile(UClass* InClass, const FString& FilePath, UObject* Outer, uint8 InFlags, const FName& CharacterCreatorName)
{
	FString CharacterCreatorJsonString;
	FFileHelper::LoadFileToString(CharacterCreatorJsonString, *FilePath);
	return LoadFromJsonString(InClass, CharacterCreatorJsonString, Outer, InFlags, CharacterCreatorName);
}

UCharacterCreator* UCharacterCreationJsonUtils::LoadFromJsonString(UClass* InClass, const FString& JsonObjString, UObject* Outer, uint8 InFlags, const FName& CharacterCreatorName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonObjString);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	return LoadFromJson(InClass, JsonObject, Outer, InFlags, CharacterCreatorName);
}

UCharacterCreator* UCharacterCreationJsonUtils::LoadFromJson(UClass* InClass, const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, uint8 InFlags, const FName& CharacterCreatorName, const bool bOverridesId, const int32 OverrideId)
{
	//Used to create assets
	const bool bIsCreatingAsset = Outer && Outer->IsA<UPackage>();
	//End Used to create assets

	UCharacterCreator* CharacterCreator = NewObject<UCharacterCreator>(Outer, UCharacterCreator::StaticClass(), bIsCreatingAsset ? CharacterCreatorName : NAME_None, static_cast<EObjectFlags>(InFlags));

	const FString& ModelID = JsonObject->GetStringField(TEXT("Model"));
	CharacterCreator->Model = LoadObject<UCharacterCreatorModel>(nullptr, *ModelID);

	CharacterCreator->DatabaseId = bOverridesId ? OverrideId : JsonObject->GetNumberField(TEXT("Id"));
	CharacterCreator->OwnerName = FString::FromInt(CharacterCreator->DatabaseId);
	CharacterCreator->BodyType = static_cast<FCharacterCreationBodyType>(JsonObject->GetIntegerField(TEXT("BodyType")));

	//Morph Target Attributes
	for (const TSharedPtr<FJsonValue>& AttributeJsonValue : JsonObject->GetArrayField(TEXT("AttributeValues")))
	{
		const TSharedPtr<FJsonObject>& AttributeJson = AttributeJsonValue->AsObject();
		const FString& AttributeId = AttributeJson->GetStringField(TEXT("Attribute"));
		const UCharacterCreatorAttribute* Attribute = LoadObject<UCharacterCreatorAttribute>(nullptr, *AttributeId);
		if (!Attribute)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorAttribute with ID: %s"), *AttributeId);
			continue;
		}
		const float& AttributeValue = AttributeJson->GetNumberField(TEXT("Value"));
		CharacterCreator->AttributeValues.Emplace(Attribute, AttributeValue);
	}

	//Pose Attributes
	for (const TSharedPtr<FJsonValue>& AttributeJsonValue : JsonObject->GetArrayField(TEXT("PoseAttributeValues")))
	{
		const TSharedPtr<FJsonObject>& AttributeJson = AttributeJsonValue->AsObject();
		const FString& AttributeId = AttributeJson->GetStringField(TEXT("Attribute"));
		const UCharacterCreatorPoseAttribute* Attribute = LoadObject<UCharacterCreatorPoseAttribute>(nullptr, *AttributeId);
		if (!Attribute)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorPoseAttribute with ID: %s"), *AttributeId);
			continue;
		}
		const float& AttributeValue = AttributeJson->GetNumberField(TEXT("Value"));
		CharacterCreator->PoseAttributeValues.Emplace(Attribute, AttributeValue);
	}

	//Material Attributes
	for (const TSharedPtr<FJsonValue>& AttributeJsonValue : JsonObject->GetArrayField(TEXT("MaterialAttributeValues")))
	{
		const TSharedPtr<FJsonObject>& AttributeJson = AttributeJsonValue->AsObject();
		const FString& AttributeId = AttributeJson->GetStringField(TEXT("MaterialAttribute"));

		UCharacterCreatorMatAttribute* Attribute = LoadObject<UCharacterCreatorMatAttribute>(nullptr, *AttributeId);
		if (!Attribute)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorMaterialAttribute with ID: %s"), *AttributeId);
			continue;
		}

		const float& AttributeValue = AttributeJson->GetNumberField(TEXT("Value"));

		TArray<const UCharacterCreatorOutfitSlot*> AffectedSlots;
		for (const TSharedPtr<FJsonValue>& AffectedSlotsJsonValue : AttributeJson->GetArrayField(TEXT("AffectedSlots")))
		{
			const FString& SlotID = AffectedSlotsJsonValue->AsString();
			const UCharacterCreatorOutfitSlot* Slot = LoadObject<UCharacterCreatorOutfitSlot>(nullptr, *SlotID);

			AffectedSlots.Add(Slot);
		}
		CharacterCreator->MaterialAttributeValues.Emplace(Attribute, AttributeValue, AffectedSlots);
	}

	//Vector Material Attributes
	for (const TSharedPtr<FJsonValue>& AttributeJsonValue : JsonObject->GetArrayField(TEXT("VectorMaterialAttributeValues")))
	{
		const TSharedPtr<FJsonObject>& AttributeJson = AttributeJsonValue->AsObject();
		const FString& AttributeId = AttributeJson->GetStringField(TEXT("MaterialAttribute"));

		UCharacterCreatorVectorMatAttribute* Attribute = LoadObject<UCharacterCreatorVectorMatAttribute>(nullptr, *AttributeId);
		if (!Attribute)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorVectorMatAttribute with ID: %s"), *AttributeId);
			continue;
		}

		const TArray<TSharedPtr<FJsonValue>> ValueJsonArray = AttributeJson->GetArrayField(TEXT("Value"));

		const FLinearColor LinearColor = FLinearColor(ValueJsonArray[0]->AsNumber(), ValueJsonArray[1]->AsNumber(), ValueJsonArray[2]->AsNumber(), ValueJsonArray[3]->AsNumber());

		TArray<const UCharacterCreatorOutfitSlot*> AffectedSlots;
		for (const TSharedPtr<FJsonValue>& AffectedSlotsJsonValue : AttributeJson->GetArrayField(TEXT("AffectedSlots")))
		{
			const FString& SlotID = AffectedSlotsJsonValue->AsString();
			const UCharacterCreatorOutfitSlot* Slot = LoadObject<UCharacterCreatorOutfitSlot>(nullptr, *SlotID);

			AffectedSlots.Add(Slot);
		}
		CharacterCreator->VectorMaterialAttributeValues.Emplace(Attribute, LinearColor, AffectedSlots);
	}

	//Outfits
	for (const TSharedPtr<FJsonValue>& OutfitJsonValue : JsonObject->GetArrayField(TEXT("SlotAndOutfitArray")))
	{
		const TSharedPtr<FJsonObject>& OutfitJson = OutfitJsonValue->AsObject();
		const FString& SlotID = OutfitJson->GetStringField(TEXT("Slot"));
		const UCharacterCreatorOutfitSlot* Slot = LoadObject<UCharacterCreatorOutfitSlot>(nullptr, *SlotID);
		if (!Slot)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorOutfitSlot with ID: %s"), *SlotID);
			continue;
		}
		//Validation model slot check
		if (CharacterCreator->Model && !CharacterCreator->Model->ContainsSlot(Slot))
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("The UCharacterCreatorOutfitSlot with ID: %s is not valid for the current model"), *SlotID);
			continue;
		}

		const FString& OutfitID = OutfitJson->GetStringField(TEXT("Outfit"));

		const UCharacterCreatorOutfit* Outfit = LoadObject<UCharacterCreatorOutfit>(nullptr, *OutfitID);
		if (!Outfit)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorOutfit with ID: %s"), *OutfitID);
			continue;
		}

		const int32& MaterialVariantIndex = OutfitJson->HasField(TEXT("MaterialVariantIndex")) ? OutfitJson->GetIntegerField(TEXT("MaterialVariantIndex")) : INDEX_NONE;
		CharacterCreator->SlotAndOutfitArray.Emplace(Slot, Outfit, MaterialVariantIndex);
	}

	//Grooms
	for (const TSharedPtr<FJsonValue>& GroomJsonValue : JsonObject->GetArrayField(TEXT("SlotAndGroomArray")))
	{
		const TSharedPtr<FJsonObject>& GroomJson = GroomJsonValue->AsObject();
		const FString& SlotID = GroomJson->GetStringField(TEXT("Slot"));
		const UCharacterCreatorOutfitSlot* Slot = LoadObject<UCharacterCreatorOutfitSlot>(nullptr, *SlotID);
		if (!Slot)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorGroomSlot with ID: %s"), *SlotID);
			continue;
		}

		//Validation model slot check
		if (CharacterCreator->Model && !CharacterCreator->Model->ContainsSlot(Slot))
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("The UCharacterCreatorOutfitSlot with ID: %s is not valid for the current model"), *SlotID);
			continue;
		}

		const FString& GroomID = GroomJson->GetStringField(TEXT("Groom"));

		UCharacterCreatorGroom* Groom = LoadObject<UCharacterCreatorGroom>(nullptr, *GroomID);
		if (!Groom)
		{
			UE_LOG(CharacterCreationLog, Error, TEXT("Can't load UCharacterCreatorGroom with ID: %s"), *GroomID);
			continue;
		}
		CharacterCreator->SlotAndGroomArray.Emplace(Slot, Groom);
	}

	return CharacterCreator;
}

TSharedPtr<FJsonObject> UCharacterCreationJsonUtils::ToJson(const UCharacterCreator* CharacterCreator)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	PopulateJson(CharacterCreator, JsonObject); // Pass the newly created JsonObject for population
	return JsonObject;
}

void UCharacterCreationJsonUtils::PopulateJson(const UCharacterCreator* CharacterCreator, TSharedPtr<FJsonObject>& JsonObject)
{
	if (!CharacterCreator)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreationStatics::ToJson() Character Creator is nullptr"));
		return;
	}

	JsonObject->SetStringField(TEXT("Id"), CharacterCreator->OwnerName);

	{
		//SlotAndOutfit
		TArray<TSharedPtr<FJsonValue>> SlotAndOutfitArray;
		for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
		{
			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("Slot"), GetPathNameSafe(SlotAndOutfit.Slot));
			JsonObjectEntry->SetStringField(TEXT("Outfit"), GetPathNameSafe(SlotAndOutfit.Outfit));
			JsonObjectEntry->SetNumberField(TEXT("MaterialVariantIndex"), SlotAndOutfit.MaterialVariantIndex);
			SlotAndOutfitArray.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}

		JsonObject->SetArrayField(TEXT("SlotAndOutfitArray"), SlotAndOutfitArray);
	} //End: SlotAndOutfit

	{
		//SlotAndGroom
		TArray<TSharedPtr<FJsonValue>> SlotAndGroomArray;
		for (const FCCSlotAndGroom& SlotAndGroom : CharacterCreator->SlotAndGroomArray)
		{
			//Validation model slot check //Do we need to check when saving and loading? Maybe not, but the system is more reliable this way
			if (CharacterCreator->Model && !CharacterCreator->Model->ContainsSlot(SlotAndGroom.Slot))
			{
				FString const SlotID = GetPathNameSafe(SlotAndGroom.Slot);
				UE_LOG(CharacterCreationLog, Error, TEXT("The UCharacterCreatorOutfitSlot with ID: %s is not valid for the current model"), *SlotID);
				continue;
			}

			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("Slot"), GetPathNameSafe(SlotAndGroom.Slot));
			JsonObjectEntry->SetStringField(TEXT("Groom"), GetPathNameSafe(SlotAndGroom.Groom));
			SlotAndGroomArray.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}

		JsonObject->SetArrayField(TEXT("SlotAndGroomArray"), SlotAndGroomArray);
	} //End: SlotAndGroom

	{
		//AttributeValues
		TArray<TSharedPtr<FJsonValue>> AttributeValues;
		for (const FCCAttributeValue& AttributeValue : CharacterCreator->AttributeValues)
		{
			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("Attribute"), GetPathNameSafe(AttributeValue.Attribute));
			JsonObjectEntry->SetNumberField(TEXT("Value"), AttributeValue.Value);
			AttributeValues.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}
		JsonObject->SetArrayField(TEXT("AttributeValues"), AttributeValues);
	} //End: AttributeValues

	{
		//PoseAttributeValues
		TArray<TSharedPtr<FJsonValue>> PoseAttributeValues;
		for (const FCCPoseAttributeValue& AttributeValue : CharacterCreator->PoseAttributeValues)
		{
			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("Attribute"), GetPathNameSafe(AttributeValue.Attribute));
			JsonObjectEntry->SetNumberField(TEXT("Value"), AttributeValue.Value);
			PoseAttributeValues.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}
		JsonObject->SetArrayField(TEXT("PoseAttributeValues"), PoseAttributeValues);
	} //End: PoseAttributeValues

	{
		//MaterialAttributeValues
		TArray<TSharedPtr<FJsonValue>> MaterialAttributeValues;
		for (const FCCMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->MaterialAttributeValues)
		{
			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("MaterialAttribute"), GetPathNameSafe(MaterialAttributeValue.MaterialAttribute));
			JsonObjectEntry->SetNumberField(TEXT("Value"), MaterialAttributeValue.Value);

			TArray<TSharedPtr<FJsonValue>> AffectedSlots;
			for (const UCharacterCreatorOutfitSlot* AffectedSlot : MaterialAttributeValue.AffectedSlots)
			{
				AffectedSlots.Add(MakeShareable(new FJsonValueString(GetPathNameSafe(AffectedSlot))));
			}
			JsonObjectEntry->SetArrayField(TEXT("AffectedSlots"), AffectedSlots);

			MaterialAttributeValues.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}
		JsonObject->SetArrayField(TEXT("MaterialAttributeValues"), MaterialAttributeValues);
	} //End: MaterialAttributeValues
	
	{
		//VectorMaterialAttributeValues
		TArray<TSharedPtr<FJsonValue>> VectorMaterialAttributeValues;
		for (const FCCVectorMaterialAttributeValue& VectorMaterialAttributeValue : CharacterCreator->VectorMaterialAttributeValues)
		{
			const TSharedPtr<FJsonObject> JsonObjectEntry = MakeShareable(new FJsonObject());
			JsonObjectEntry->SetStringField(TEXT("MaterialAttribute"), GetPathNameSafe(VectorMaterialAttributeValue.MaterialAttribute));

			const TArray<TSharedPtr<FJsonValue>> ValueJsonArray =
			{
				MakeShareable(new FJsonValueNumber(VectorMaterialAttributeValue.Value.R)),
				MakeShareable(new FJsonValueNumber(VectorMaterialAttributeValue.Value.G)),
				MakeShareable(new FJsonValueNumber(VectorMaterialAttributeValue.Value.B)),
				MakeShareable(new FJsonValueNumber(VectorMaterialAttributeValue.Value.A))
			};
			JsonObjectEntry->SetArrayField(TEXT("Value"), ValueJsonArray);
			
			TArray<TSharedPtr<FJsonValue>> AffectedSlots;
			for (const UCharacterCreatorOutfitSlot* AffectedSlot : VectorMaterialAttributeValue.AffectedSlots)
			{
				AffectedSlots.Add(MakeShareable(new FJsonValueString(GetPathNameSafe(AffectedSlot))));
			}
			JsonObjectEntry->SetArrayField(TEXT("AffectedSlots"), AffectedSlots);

			VectorMaterialAttributeValues.Add(MakeShareable(new FJsonValueObject(JsonObjectEntry)));
		}
		JsonObject->SetArrayField(TEXT("VectorMaterialAttributeValues"), VectorMaterialAttributeValues);
	} //End: VectorMaterialAttributeValue

	{
		//BodyType
		JsonObject->SetNumberField(TEXT("BodyType"), (double)CharacterCreator->BodyType); //Will this cause errors due to int to double conversion?
	} //End: BodyType

	{
		//Validation Model
		JsonObject->SetStringField(TEXT("Model"), GetPathNameSafe(CharacterCreator->Model));
	} //End: Validation Model
}
