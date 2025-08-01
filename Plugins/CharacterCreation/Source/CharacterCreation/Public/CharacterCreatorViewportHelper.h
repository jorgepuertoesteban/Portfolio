// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterCreatorViewportHelper.generated.h"

class UCharacterCreatorComponent;
class USkeletalMeshComponent;
class UGroomComponent;

UCLASS()
class CHARACTERCREATION_API ACharacterCreatorViewportHelper : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalComp;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FaceMesh;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BottomMesh;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ShoesMesh;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> HairGroom;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> EyebrowsGroom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> EyelashesGroom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> MoustacheGroom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> BeardGroom;
	
	UPROPERTY(Category = Character,  VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterCreatorComponent> CharacterCreatorComp;

public:

	static FName CapsuleComponentName;
	static FName MeshComponentName;
	static FName FaceMeshComponentName;
	static FName BodyMeshComponentName;
	static FName BottomMeshComponentName;
	static FName ShoesMeshComponentName;
	static FName HairGroomComponentName;
	static FName EyebrowsGroomComponentName;
	static FName EyelashesGroomComponentName;
	static FName MoustacheGroomComponentName;
	static FName BeardGroomComponentName;

public:	

	ACharacterCreatorViewportHelper();

	FORCEINLINE class USkeletalMeshComponent* GetFaceMesh() const { return FaceMesh; }
	FORCEINLINE class USkeletalMeshComponent* GetBodyMesh() const { return BodyMesh; }
	FORCEINLINE class USkeletalMeshComponent* GetBottomMesh() const { return BottomMesh; }
	FORCEINLINE class USkeletalMeshComponent* GetShoesMesh() const { return ShoesMesh; }

	FORCEINLINE class UGroomComponent* GetHairGroom() const { return HairGroom; }
	FORCEINLINE class UGroomComponent* GetEyebrowsGroom() const { return EyebrowsGroom; }
	FORCEINLINE class UGroomComponent* GetEyelashesGroom() const { return EyelashesGroom; }
	FORCEINLINE class UGroomComponent* GetMoustacheGroom() const { return MoustacheGroom; }
	FORCEINLINE class UGroomComponent* GetBeardGroom() const { return BeardGroom; }

	FORCEINLINE class UCharacterCreatorComponent* GetCharacterCreatorComponent() const { return CharacterCreatorComp; }

};
