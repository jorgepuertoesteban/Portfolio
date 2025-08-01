// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MappableActionTraditional.generated.h"

USTRUCT(BlueprintType)
struct FMappableActionTraditional
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere, meta=(ClampMin="-1"))
	int32 KeyIndex;

	FMappableActionTraditional()
	{
		Name = "";
		KeyIndex = -1;
	}

	FMappableActionTraditional(const FName& InActionName, const int32 InKeyIndex)
	{
		Name = InActionName;
		KeyIndex = InKeyIndex;
	}

	bool IsValid() const
	{
		return !Name.IsEqual("") && KeyIndex > 0;
	}
	
	bool operator==(const FMappableActionTraditional& Other) const
	{
		return Name.IsEqual(Other.Name) && KeyIndex == Other.KeyIndex;
	}
	
	bool operator!=(const FMappableActionTraditional& Other) const
	{
		return !(*this == Other);
	}
	
	friend FORCEINLINE uint32 GetTypeHash(const FMappableActionTraditional& Action)
	{
		return FCrc::MemCrc32(&Action, sizeof(FMappableActionTraditional));
	}

};
