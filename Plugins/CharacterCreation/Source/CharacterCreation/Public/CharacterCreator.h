// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreator.generated.h"

class UCharacterCreatorAttributesSet;
class UCharacterCreatorAttribute;
class UCharacterCreatorPoseAttribute;
class UCharacterCreatorOutfit;
class UCharacterCreatorOutfitSlot;
class UCharacterCreatorVectorMatAttribute;
class UCharacterCreatorGroom;
class UCharacterCreatorModel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCharacterCreatorOutfitChanged, const UCharacterCreatorOutfit*, Outfit, FName, SlotName, const int32, MaterialVariantIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterCreatorGroomChanged, const UCharacterCreatorGroom*, Groom, FName, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreatorBodyTypeChanged, const FCharacterCreationBodyType, NewBodyType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterCreatorAttributeChanged, const UCharacterCreatorAttribute*, Attribute, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterCreatorMaterialAttributeChanged, const UCharacterCreatorMatAttribute*, MaterialAttribute, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCCVectorMaterialAttributeChanged, const UCharacterCreatorVectorMatAttribute*, MaterialAttribute, FLinearColor, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterCreatorPoseAttributeChanged, const UCharacterCreatorPoseAttribute*, PoseAttribute, float, Value);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreator : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	int32 DatabaseId;

	UPROPERTY(VisibleAnywhere)
	FString OwnerName;

	UPROPERTY(VisibleAnywhere)
	FName RootSkeletalMeshTag = TEXT("RootSkeletalMesh");

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<FCCSlotAndOutfit> SlotAndOutfitArray;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<FCCSlotAndGroom> SlotAndGroomArray;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<FCCAttributeValue> AttributeValues;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	TArray<FCCPoseAttributeValue> PoseAttributeValues;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = MaterialAttributes)
	TArray<FCCMaterialAttributeValue> MaterialAttributeValues;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = MaterialAttributes)
	TArray<FCCVectorMaterialAttributeValue> VectorMaterialAttributeValues;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	FCharacterCreationBodyType BodyType;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = CharacterCreator)
	UCharacterCreatorModel* Model;

	UPROPERTY(Transient)
	FOnCharacterCreatorOutfitChanged OnOutfitChangedDelegate;

	UPROPERTY(Transient)
	FOnCharacterCreatorGroomChanged OnGroomChangedDelegate;

	UPROPERTY(Transient)
	FOnCharacterCreatorAttributeChanged OnAttributeChangedDelegate;
	
	UPROPERTY(Transient)
	FOnCharacterCreatorMaterialAttributeChanged OnMaterialAttributeChangedDelegate;

	UPROPERTY(Transient)
	FOnCCVectorMaterialAttributeChanged OnVectorMaterialAttributeChangedDelegate;

	UPROPERTY(Transient)
	FOnCharacterCreatorPoseAttributeChanged OnPoseAttributeChangedDelegate;

	UPROPERTY(Transient)
	FOnCharacterCreatorBodyTypeChanged OnBodyTypeChangedDelegate;

public:

	FORCEINLINE AActor* GetOwningActor() const { return Cast<AActor>(GetOuter()); }

	FORCEINLINE UActorComponent* GetOwningComponent() const { return Cast<UActorComponent>(GetOuter()); }

	//Functions Needed to replicate, from UObject
	virtual bool IsSupportedForNetworking() const override { return true; }
	//virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags); // note no override because this is the FIRST declaration of this function.
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual UWorld* GetWorld() const override;
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	//End Functions Needed to replicate, from UObject

	float ValueOf(const UCharacterCreatorAttribute* CCAttribute);
	
	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	float ValueOf(const UCharacterCreatorMatAttribute* CCAttribute);

	UFUNCTION(BlueprintCallable)
	void ClearCharacterCreator();

	UFUNCTION(BlueprintCallable)
	void SetAttributeValue(const UCharacterCreatorAttribute* Attribute, float NewValue);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttributeChanged(const UCharacterCreatorAttribute* Attribute, float NewValue);

	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UFUNCTION(BlueprintCallable)
	void SetMaterialAttributeValue(const UCharacterCreatorMatAttribute* Attribute, float NewValue);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MaterialAttributeChanged(const UCharacterCreatorMatAttribute* Attribute, float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetVectorMaterialAttributeValue(const UCharacterCreatorVectorMatAttribute* Attribute, FLinearColor NewValue);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_VectorMaterialAttributeChanged(const UCharacterCreatorVectorMatAttribute* Attribute, FLinearColor NewValue);
	
	UFUNCTION(BlueprintCallable)
	void SetPoseAttributeValue(const UCharacterCreatorPoseAttribute* Attribute, float NewValue);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PoseAttributeChanged(const UCharacterCreatorPoseAttribute* Attribute, float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetMaterialAttributeAffectedSlots(const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MaterialAttributeAffectedSlotChanged(const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue);

	UFUNCTION(BlueprintCallable)
	const UCharacterCreatorOutfit* GetSelectedOutfit(const UCharacterCreatorOutfitSlot* OutfitSlot) const;

	UFUNCTION(BlueprintCallable)
	void SetOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex = -1);

	UFUNCTION(BlueprintCallable)
	bool IsUsingOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex = -1) const;

	UFUNCTION(BlueprintCallable)
	void ClearOutfitSlot(const UCharacterCreatorOutfitSlot* OutfitSlot);


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OutfitChanged(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex = -1);

	UFUNCTION(BlueprintCallable)
	const UCharacterCreatorGroom* GetSelectedGroom(const UCharacterCreatorOutfitSlot* OutfitSlot);
	
	UFUNCTION(BlueprintCallable)
	void SetGroom(const UCharacterCreatorGroom* Groom);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GroomChanged(const UCharacterCreatorGroom* Groom, FName SlotName);

	FCharacterCreationBodyType GetSelectedBodyType() const;
	void SetBodyType(FCharacterCreationBodyType NewBodyType);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BodyTypeChanged(FCharacterCreationBodyType NewBodyType);

	void SetCharacterCreatorMaterialVariantIndex(int32 NewIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CharacterCreatorMaterialVariantIndex(int32 NewIndex);

	
	// Overrides
	/*
	virtual bool IsNameStableForNetworking() const override;
	virtual void PreNetReceive() override;
	virtual void PostNetReceive() override;*/

	//UFUNCTION()
	//void OnRepSlotAndOutfitArray();
	//


};
