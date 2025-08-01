// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCreationTypes.h"
#include "Interfaces/CharacterCreationDAO.h"
#include "Engine/StreamableManager.h"
#include "CharacterCreatorComponent.generated.h"

class UCharacterCreator;
class USkeletalMeshComponent;
class UGroomComponent;
class UCharacterCreatorVectorMatAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOutfitChanged, UMeshComponent*, MeshComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroomChanged, UMeshComponent*, MeshComponent);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static FName GetDynamicCharacterCreatorComponentTag() { return DynamicCharacterCreatorComponentTag; }

protected:
	static FName DynamicCharacterCreatorComponentTag;

	UPROPERTY(ReplicatedUsing = "OnRep_CharacterCreator", EditAnywhere, BlueprintReadOnly, Category = "CharacterCreator")
	UCharacterCreator* CharacterCreator = nullptr;

	/**
	 * It's used for clients so it's not overridden by Server
	 */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterCreator")
	bool bSyncWithControlComponent = false;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "CharacterCreator")
	UCharacterCreator* CharacterCreatorLastUsed = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly)
	TMap<const UCharacterCreatorOutfitSlot*, USkeletalMeshComponent*> SlotSKMeshMap;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TMap<const UCharacterCreatorOutfitSlot*, UGroomComponent*> SlotGroomMap;


	//Slot mesh map contains duplicated data as it is a concatenation of slots and grooms used to share material attributes
	UPROPERTY(Transient, BlueprintReadOnly)
	TMap<const UCharacterCreatorOutfitSlot*, UMeshComponent*> SlotMeshMap;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterCreator")
	USkeletalMeshComponent* RootSkeletalMesh = nullptr;

	TSharedPtr<FStreamableHandle> ItemStreamingHandle;

	virtual UGroomComponent* FindOrCreateGroomComponent(const UCharacterCreatorGroom* NewGroom, const FName& SlotName, bool& bFiltered);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterCreator")
	int32 DatabaseId = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreator")
	TSubclassOf<USkeletalMeshComponent> SkeletalMeshClass = USkeletalMeshComponent::StaticClass();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreator")
	bool bSearchSkeletalMeshesInActor = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreator")
	bool bSearchGroomsInActor = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreator")
	TArray<FName> SlotsToFilterOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterCreator")
	FCharacterCreationBodyParts BodyPartsComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterCreator")
	const UCharacterCreatorOutfitSlot* FaceSlot = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterCreator")
	USkeletalMeshComponent* FaceSkeletalMesh = nullptr;
	
	UPROPERTY(BlueprintAssignable)
	FOnOutfitChanged OnOutfitChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGroomChanged OnGroomChanged;

	UPROPERTY(Transient)
	TMap<const UCharacterCreatorOutfitSlot* , FDuplicatedSkeletalMesh> DuplicatedSkeletalMeshes;

public:

	UCharacterCreatorComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintCallable)
	UCharacterCreator* GetCharacterCreator() const { return CharacterCreator; }

	UFUNCTION(BlueprintCallable)
	virtual void SetRootSkeletalMesh(USkeletalMeshComponent* NewRootSkComp);

	UFUNCTION(BlueprintPure, Category = "Viewport")
	static UCharacterCreatorComponent* GetCharacterCreatorComponent(const AActor* ActorOwner);

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshBySlot(const UCharacterCreatorOutfitSlot* Slot) const
	{
		if (SlotSKMeshMap.Contains(Slot))
		{
			return SlotSKMeshMap[Slot];
		}
		return nullptr;
	}

	UFUNCTION()
	void OnRep_CharacterCreator();

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	void SetCharacterCreator(UCharacterCreator* NewCharacterCreator, bool bReload = true);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	bool IsUsingOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex = -1) const;

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	void SetOutfit(const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	void SetGroom(const UCharacterCreatorGroom* NewGroom, const FName& SlotName);

	UFUNCTION()
	void SetMorphTarget(const UCharacterCreatorAttribute* Attribute, const float& Value) const;
	
	UFUNCTION()
	void SetMaterialAttribute(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value);

	UFUNCTION()
	void SetVectorMaterialAttribute(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value);

	UFUNCTION()
	void SetPoseAttribute(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	bool LoadCharacterCreatorFromDatabase();

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	bool SaveCharacterCreatorToDatabase();

	UFUNCTION(BlueprintCallable, Category = "CharacterCreator")
	void OnLoadDaoResponse(const FLoadCharacterCreatorResponse& Response);

	UFUNCTION()
	void OnChangedReceived();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ReloadCurrentCharacterCreator();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ReloadCurrentCharacterCreatorWithAsyncLoad();

	UFUNCTION()
	void OnOutfitChangedReceived(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex = -1);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnOutfitChangedReceived(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex = -1);

	UFUNCTION()
	void OnGroomChangedReceived(const UCharacterCreatorGroom* Groom, FName SlotName);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnGroomChangedReceived(const UCharacterCreatorGroom* Groom, FName SlotName);

	UFUNCTION()
	void OnAttributeChangedReceived(const UCharacterCreatorAttribute* Attribute, float Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnAttributeChangedReceived(const UCharacterCreatorAttribute* Attribute, float Value);

	UFUNCTION()
	void OnMaterialAttributeChangedReceived(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnMaterialAttributeChangedReceived(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value);

	UFUNCTION()
	void OnVectorMaterialAttributeChangedReceived(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnVectorMaterialAttributeChangedReceived(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value);

	UFUNCTION()
	void OnPoseAttributeChangedReceived(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPoseAttributeChangedReceived(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value);

	UFUNCTION()
	void OnBodyTypeChangedReceived(FCharacterCreationBodyType NewBodyType);

private:
	void SetDefaultRootSkeletalMesh();

	void OutfitLoadAsync(TArray<FSoftObjectPath>& AssetsToStream, const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex = -1);
	void SetOutfitLoadedAsset(const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex = -1);
	void SetGroomLoadedAsset(const UCharacterCreatorGroom* NewGroom, const FName& SlotName);

	static bool HasValidUVSets(const USkeletalMesh* SkeletalMesh);
};
