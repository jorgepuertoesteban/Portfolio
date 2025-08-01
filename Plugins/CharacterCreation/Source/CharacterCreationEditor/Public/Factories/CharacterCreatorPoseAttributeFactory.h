// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CharacterCreatorPoseAttributeFactory.generated.h"

class UCharacterCreatorPoseAttribute;
class USkeletalMesh;

UCLASS()
class CHARACTERCREATIONEDITOR_API UCharacterCreatorPoseAttributeFactory : public UFactory
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	USkeletalMesh* BaseSkeletalMesh;

	UPROPERTY(BlueprintReadWrite)
	USkeletalMesh* MinValueSkeletalMesh;

	UPROPERTY(BlueprintReadWrite)
	USkeletalMesh* MaxValueSkeletalMesh;

	UCharacterCreatorPoseAttributeFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override { return true; }
	virtual bool ConfigureProperties() override;
	// End of UFactory interface

private:
	void OnWindowUserActionDelegate(bool bCreate, USkeletalMesh* BaseSkeletalMesh, USkeletalMesh* MinValueSkeletalMesh, USkeletalMesh* MaxValueSkeletalMesh);
};
