// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "CharacterCreationJsonUtils.generated.h"

class UCharacterCreator;


UCLASS()
class CHARACTERCREATION_API UCharacterCreationJsonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Character Creation")
	static UCharacterCreator* LoadFromJsonFile(UClass* InClass, const FString& FilePath, UObject* Outer, uint8 InFlags = 0, const FName& CharacterCreatorName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Character Creation")
	static UCharacterCreator* LoadFromJsonString(UClass* InClass, const FString& JsonObjString, UObject* Outer, uint8 InFlags = 0, const FName& CharacterCreatorName = NAME_None);

	//UFUNCTION(BlueprintCallable, Category = "Character Creation")
	static UCharacterCreator* LoadFromJson(UClass* InClass, const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, uint8 InFlags = 0, const FName& CharacterCreatorName = NAME_None, const bool bOverridesId = false, const int32 OverrideId = 0);

	static TSharedPtr<FJsonObject> ToJson(const UCharacterCreator* CharacterCreator);
	static void PopulateJson(const UCharacterCreator* CharacterCreator, TSharedPtr<FJsonObject>& JsonObject);
};
