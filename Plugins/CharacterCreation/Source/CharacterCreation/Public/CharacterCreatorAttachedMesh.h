// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorAttachedMesh.generated.h"

class UStaticMesh;
class UCharacterCreatorOutfitSlot;
class UTexture;

UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorAttachedMesh : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FName AttachedMeshName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FText FriendlyName;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	int32 DatabaseId;
	
};
