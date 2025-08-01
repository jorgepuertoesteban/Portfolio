// Created by Jorge Puerto. All rights reserved.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterCreationTypes.generated.h"

class USkeletalMeshComponent;
class USkeletalMesh;
class UCharacterCreatorOutfitSlot;
class UCharacterCreatorOutfit;
class UCharacterCreatorGroom;
class UCharacterCreatorAttribute;
class UCharacterCreatorMatAttribute;
class UCharacterCreatorVectorMatAttribute;
class UCharacterCreatorPoseAttribute;
class UCharacterCreatorOutfitModifier;
class UCharacterCreatorCondition;
class UCharacterCreatorTextureMatAttribute;
class UTexture;

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FDuplicatedSkeletalMesh
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const USkeletalMesh* Original;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	USkeletalMesh* Duplication;

	FDuplicatedSkeletalMesh(): Original(nullptr), Duplication(nullptr){}
	FDuplicatedSkeletalMesh(const USkeletalMesh* NewOriginal, USkeletalMesh* NewDuplication): Original(NewOriginal), Duplication(NewDuplication){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCSlotAndOutfit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorOutfit* Outfit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	int32 MaterialVariantIndex = INDEX_NONE;

	
	FCCSlotAndOutfit() : Slot(nullptr), Outfit(nullptr) {}

	FCCSlotAndOutfit(const UCharacterCreatorOutfitSlot* NewSlot, const UCharacterCreatorOutfit* NewValue, const int32 NewMaterialVariantIndex) : Slot(NewSlot), Outfit(NewValue), MaterialVariantIndex(NewMaterialVariantIndex){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCSlotAndGroom
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorOutfitSlot* Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorGroom* Groom;

	FCCSlotAndGroom() : Slot(nullptr), Groom(nullptr) {}

	FCCSlotAndGroom(const UCharacterCreatorOutfitSlot* NewSlot, const UCharacterCreatorGroom* NewValue) : Slot(NewSlot), Groom(NewValue) {}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorAttribute* Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	float Value;


	FCCAttributeValue() :Attribute(nullptr), Value(0.f) {}

	FCCAttributeValue(const UCharacterCreatorAttribute* NewAttribute, float NewValue) :Attribute(NewAttribute), Value(NewValue) {}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCPoseAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorPoseAttribute* Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	float Value;

	FCCPoseAttributeValue() :Attribute(nullptr), Value(0.f) {}

	FCCPoseAttributeValue(const UCharacterCreatorPoseAttribute* NewAttribute, float NewValue) :Attribute(NewAttribute), Value(NewValue){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCMaterialAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorMatAttribute* MaterialAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<const UCharacterCreatorOutfitSlot*> AffectedSlots;

	//FCCMaterialAttributeValue() :MaterialAttribute(nullptr), Value(0.f){}
	FCCMaterialAttributeValue() :MaterialAttribute(nullptr), Value(0.f), AffectedSlots(){}

	//Legacy, migth dissapear
	FCCMaterialAttributeValue(const UCharacterCreatorMatAttribute* NewAttribute, float NewValue) :MaterialAttribute(NewAttribute), Value(NewValue){}
	FCCMaterialAttributeValue(const UCharacterCreatorMatAttribute* NewAttribute, float NewValue, TArray<const UCharacterCreatorOutfitSlot*> NewAffectedSlots) :MaterialAttribute(NewAttribute), Value(NewValue), AffectedSlots(NewAffectedSlots){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCTextureMaterialAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorTextureMatAttribute* TextureMaterialAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	UTexture* Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<const UCharacterCreatorOutfitSlot*> AffectedSlots;

	FCCTextureMaterialAttributeValue() :TextureMaterialAttribute(nullptr), Value(nullptr), AffectedSlots(){}

	//Legacy, migth dissapear
	FCCTextureMaterialAttributeValue(const UCharacterCreatorTextureMatAttribute* NewAttribute, UTexture* NewValue) :TextureMaterialAttribute(NewAttribute), Value(NewValue){}
	FCCTextureMaterialAttributeValue(const UCharacterCreatorTextureMatAttribute* NewAttribute, UTexture* NewValue, TArray<const UCharacterCreatorOutfitSlot*> NewAffectedSlots) :TextureMaterialAttribute(NewAttribute), Value(NewValue), AffectedSlots(NewAffectedSlots){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCCVectorMaterialAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	const UCharacterCreatorVectorMatAttribute* MaterialAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	FLinearColor Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<const UCharacterCreatorOutfitSlot*> AffectedSlots;

	//FCCMaterialAttributeValue() :MaterialAttribute(nullptr), Value(0.f){}
	FCCVectorMaterialAttributeValue() :MaterialAttribute(nullptr), Value(FLinearColor::White), AffectedSlots(){}

	//Legacy, might disappear
	FCCVectorMaterialAttributeValue(const UCharacterCreatorVectorMatAttribute* NewAttribute, FLinearColor NewValue) :MaterialAttribute(NewAttribute), Value(NewValue){}
	FCCVectorMaterialAttributeValue(const UCharacterCreatorVectorMatAttribute* NewAttribute, FLinearColor NewValue, TArray<const UCharacterCreatorOutfitSlot*> NewAffectedSlots) :MaterialAttribute(NewAttribute), Value(NewValue), AffectedSlots(NewAffectedSlots){}
};

USTRUCT(BlueprintType)
struct CHARACTERCREATION_API FCharacterCreationBodyParts
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	USkeletalMeshComponent* UpperBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	USkeletalMeshComponent* LowerBody;

	FCharacterCreationBodyParts()
	{
		Head = nullptr;
		UpperBody = nullptr;
		LowerBody = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FCCMOUnderConditions
{
	GENERATED_BODY();

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced)
	TObjectPtr<UCharacterCreatorOutfitModifier> Modifier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced)
	TArray<TObjectPtr<UCharacterCreatorCondition>> Conditions;
};

UENUM(BlueprintType, Meta = (Bitflags))
enum class FCharacterCreationBodyType : uint8
{
	NormalWeight = 0,
	OverWeight,
	UnderWeight,
	//NormalWeightF,
	MAX
};

ENUM_RANGE_BY_COUNT(FCharacterCreationBodyType, FCharacterCreationBodyType::MAX)
