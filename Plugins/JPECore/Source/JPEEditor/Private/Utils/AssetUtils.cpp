// Created by Jorge Puerto. All Rights Reserved.


#include "Utils/AssetUtils.h"

#include "AssetToolsModule.h"
#include "EditorClassUtils.h"
#include "PropertyPathHelpers.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY(LogAssetUtils);

TArray<UObject*> UAssetUtils::LoadFromJsonFile(const FString& FilePath, UFactory* AssetFactory)
{
	FString InventoryJsonString;
	FFileHelper::LoadFileToString(InventoryJsonString, *FilePath);
	return LoadFromJsonString(InventoryJsonString, AssetFactory);
}

TArray<UObject*> UAssetUtils::LoadFromJsonString(const FString& JsonObjectString, UFactory* AssetFactory)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonObjectString);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	return LoadFromJson(JsonObject, AssetFactory);
}

TArray<UObject*> UAssetUtils::LoadFromJson(const TSharedPtr<FJsonObject>& JsonObject, UFactory* AssetFactory)
{
	TArray<UObject*> Assets;
	FJsonObject* Json = JsonObject.Get();
	// Do a first that just creates the assets
	for (const TTuple<FString, TSharedPtr<FJsonValue>>& AssetJson : Json->Values)
	{
		// Extract the asset and class paths from the asset data object
		const TSharedPtr<FJsonObject>& AssetData = AssetJson.Value->AsObject();
		const FString AssetName = AssetData->GetStringField(TEXT("AssetName"));
		const FString AssetPath = AssetData->GetStringField(TEXT("AssetPath"));
		const FString AssetClassPathName = AssetData->GetStringField(TEXT("AssetClassPathName"));

		// Create asset from fetched information
		UClass* AssetClass = FEditorClassUtils::GetClassFromString(AssetClassPathName);
		FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
		UObject* Asset = AssetToolsModule.Get().CreateAsset(AssetName, AssetPath, AssetClass, AssetFactory);

		// Add the newly initialized asset to the array of assets extracted from the Json
		Assets.Add(Asset);
	}
	// Do a second loop that inits the newly created assets' properties; this is done to prevent asset reference break
	int i = 0;
	for (const TTuple<FString, TSharedPtr<FJsonValue>>& AssetJson : Json->Values)
	{
		if (!Assets.IsValidIndex(i))
		{
			break;
		}
		
		// Get the asset and its data from this iteration
		UObject* Asset = Assets[i++];
		const FString AssetName = AssetJson.Key;
		const TSharedPtr<FJsonObject>& AssetData = AssetJson.Value->AsObject();
		
		// Initialize the asset's properties
		const TSharedPtr<FJsonObject>& AssetProperties = AssetData->GetObjectField(TEXT("AssetProperties"));
		for (const TTuple<FString, TSharedPtr<FJsonValue>>& Property : AssetProperties->Values)
		{
			const FString PropertyName = Property.Key;
			const FString PropertyValueAsString = Property.Value->AsString();
			if (!PropertyPathHelpers::SetPropertyValueFromString(Asset, PropertyName, PropertyValueAsString))
			{
				UE_LOG(LogAssetUtils, Warning, TEXT("UAssetUtils::LoadFromJson: Failed trying to init asset %s's property named \"%s\" with value \"%s\"."), *AssetName, *PropertyName, *PropertyValueAsString);
			}
		}
	}
	return Assets;
}

TSharedPtr<FJsonObject> UAssetUtils::ToJson(UObject* Asset)
{
	if (!Asset)
	{
		return nullptr;
	}
	
	// Create Json and store all asset data in it
	TSharedPtr<FJsonObject> AssetJson = MakeShareable(new FJsonObject());
	FString AssetPathName, AssetName;
	Asset->GetPathName().Split(".", &AssetPathName, &AssetName);
	const FString SlashAssetName = "/" + AssetName;
	const FString AssetPath = AssetPathName.Replace(*SlashAssetName, TEXT(""));
	AssetJson->SetStringField("AssetName", AssetName);
	AssetJson->SetStringField("AssetPath", AssetPath);
#if ENGINE_MAJOR_VERSION == 4
	AssetJson->SetStringField("AssetClassPathName", Asset->GetClass()->GetFullName().Replace(TEXT("Class "), TEXT("")));
#elif ENGINE_MAJOR_VERSION == 5
	AssetJson->SetStringField("AssetClassPathName", Asset->GetClass()->GetClassPathName().ToString());
#endif
	// The properties of the asset need to be stored in a separate object
	const TSharedPtr<FJsonObject> AssetProperties = MakeShareable(new FJsonObject());
	for (TFieldIterator<FProperty> It(Asset->GetClass()); It; ++It)
	{
		const FProperty* Property = *It;
		if (!Property)
		{
			continue;
		}
		FString PropertyName = Property->GetName();
		FString PropertyValue;

		// We get the property's value as a string because we want to store it in a Json file
		PropertyPathHelpers::GetPropertyValueAsString(Asset, PropertyName, PropertyValue);
		AssetProperties->SetStringField(PropertyName, PropertyValue);
	}
	AssetJson->SetObjectField("AssetProperties", AssetProperties);
	
	return AssetJson;
}

bool UAssetUtils::WriteJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath)
{
	// Convert Json to String
	FString OutputString;
	const TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// Write resulting string to file
	return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}