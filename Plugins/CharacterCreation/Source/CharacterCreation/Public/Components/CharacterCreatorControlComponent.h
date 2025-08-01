// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreationTypes.h"
#include "Components/ControlComponent.h"
#include "Interfaces/CharacterCreationDAO.h"
#include "Misc/CoreMiscDefines.h"
#include "CharacterCreatorControlComponent.generated.h"

class ACharacterCreatorPawn;
class AThePrisonCharacterCreatorPawn;
class UCharacterCreator;
class UCharacterCreatorAttribute;
class UCharacterCreatorGroom;
class UCharacterCreatorHUDInterface;
class UCharacterCreatorMatAttribute;
class UCharacterCreatorVectorMatAttribute;
class UCharacterCreatorPoseAttribute;
class UCharacterCreatorOutfit;
class UCharacterCreatorOutfitSlot;
class UActorChannel;
class FOutBunch;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTERCREATION_API UCharacterCreatorControlComponent : public UControlComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = CharacterCreator)
	TSubclassOf<ACharacterCreatorPawn> CharacterCreatorPawnClass;

public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	TArray<UCharacterCreator*> CharacterCreators;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	UCharacterCreator* MainCharacterCreator;

public:
	DECLARE_EVENT_OneParam(UCharacterCreatorControlComponent, FOnNewMainCharacterCreatorChangedEvent, UCharacterCreator*);

	FOnNewMainCharacterCreatorChangedEvent& OnNewMainCharacterCreatorChanged() { return OnNewMainCharacterCreatorChangedEvent; }

private:
	FOnNewMainCharacterCreatorChangedEvent OnNewMainCharacterCreatorChangedEvent;

public:
	UFUNCTION(BlueprintCallable)
	static UCharacterCreatorControlComponent* GetCharacterCreatorControlComponent(const AActor* Owner);

	UFUNCTION(BlueprintCallable, Category="Character Creation")
	UCharacterCreator* GetCharacterCreator() const
	{
		return MainCharacterCreator;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddCharacterCreator(UCharacterCreator* NewCharacterCreator, bool bIsMainCC = true);

	UFUNCTION(BlueprintCallable)
	void SetAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorAttribute* CCAttribute, float NewValue);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorAttribute* CCAttribute, float NewValue);

	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UFUNCTION(BlueprintCallable)
	void SetMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, float NewValue);

	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, float NewValue);
	
	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UFUNCTION(BlueprintCallable)
	void SetVectorMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorVectorMatAttribute* CCAttribute, FLinearColor NewValue);

	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetVectorMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorVectorMatAttribute* CCAttribute, FLinearColor NewValue);

	UFUNCTION(BlueprintCallable)
	void SetPoseAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorPoseAttribute* CCPoseAttribute, float NewValue);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetPoseAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorPoseAttribute* CCPoseAttribute, float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetMaterialAttributeAffectedSlots(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue);

	/*UFUNCTION(BlueprintCallable)
	void SetMaterialAttributeAffectedSlots(UCharacterCreatorSlotDH* CCSlotDH, bool NewValue);*/

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetMaterialAttributeAffectedSlots(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue);

	UFUNCTION(BlueprintCallable)
	void SetOutfit(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit, const int32 MaterialVariantIndex = -1);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetOutfit(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit, const int32 MaterialVariantIndex = -1);

	UFUNCTION(BlueprintCallable)
	void ClearOutfitSlot(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfitSlot* OutfitSlot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ClearOutfitSlot(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfitSlot* OutfitSlot);

	UFUNCTION(BlueprintCallable)
	void ValidateGroomBindings(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ValidateGroomBindings(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit);

	UFUNCTION(BlueprintCallable)
	void SetGroom(UCharacterCreator* CharacterCreator, const UCharacterCreatorGroom* SelectedGroom);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetGroom(UCharacterCreator* CharacterCreator, const UCharacterCreatorGroom* SelectedGroom);

	UFUNCTION(BlueprintCallable)
	void SetBodyType(UCharacterCreator* CharacterCreator, FCharacterCreationBodyType NewBodyType);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetBodyType(UCharacterCreator* CharacterCreator, FCharacterCreationBodyType NewBodyType);

	UFUNCTION(BlueprintCallable)
	void SaveCharacterCreator(UCharacterCreator* CharacterCreator = nullptr);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SaveCharacterCreator(UCharacterCreator* CharacterCreator = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void ShowCharacterCreatorHUD();

	UFUNCTION(BlueprintCallable)
	virtual void HideCharacterCreatorHUD();

	UFUNCTION(BlueprintCallable, Category="Character Creation")
	virtual void SetCharacterCreator(UCharacterCreator* NewCharacterCreator);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Character Creation")
	void Server_SpawnCharacterCreatorPawn(const FTransform& Transform, const int32 CharacterCreatorIndex = 0);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Character Creation")
	void Server_DespawnCharacterCreatorPawn();
};
