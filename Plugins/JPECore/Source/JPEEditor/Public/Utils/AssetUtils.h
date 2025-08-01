// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAssetUtils, Log, All);

UCLASS()
class JPEEDITOR_API UAssetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static TArray<UObject*> LoadFromJsonFile(const FString& FilePath, UFactory* AssetFactory);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static TArray<UObject*> LoadFromJsonString(const FString& JsonObjectString, UFactory* AssetFactory);

	// UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static TArray<UObject*> LoadFromJson(const TSharedPtr<FJsonObject>& JsonObject, UFactory* AssetFactory);

	// UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static TSharedPtr<FJsonObject> ToJson(UObject* Asset);
	
	// UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static bool WriteJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath);
};
