// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MappableActionEnhanced.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FMappableActionEnhanced
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere)
	UInputAction* InputAction;
	UPROPERTY(EditAnywhere)
	FName Name;

	FMappableActionEnhanced()
	{
		InputMappingContext = nullptr;
		InputAction = nullptr;
		Name = "None";
	}

	FMappableActionEnhanced(UInputMappingContext* InInputMappingContext, UInputAction* InInputAction, const FName& InName)
	{
		InputMappingContext = InInputMappingContext;
		InputAction = InInputAction;
		Name = InName;
	}

	bool IsValid() const
	{
		return InputMappingContext && InputAction && !Name.IsEqual("");
	}
	
	bool operator==(const FMappableActionEnhanced& Other) const
	{
		return InputMappingContext == Other.InputMappingContext && InputAction == Other.InputAction && Name.IsEqual(Other.Name);
	}
	
	bool operator!=(const FMappableActionEnhanced& Other) const
	{
		return !(*this == Other);
	}
	
	friend FORCEINLINE uint32 GetTypeHash(const FMappableActionEnhanced& Action)
	{
		return FCrc::MemCrc32(&Action, sizeof(FMappableActionEnhanced));
	}

};
