// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreatorViewportHelper.h"
#include "Components/CharacterCreatorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GroomComponent.h"


FName ACharacterCreatorViewportHelper::MeshComponentName(TEXT("CharacterMesh0"));
FName ACharacterCreatorViewportHelper::CapsuleComponentName(TEXT("CollisionCylinder"));
FName ACharacterCreatorViewportHelper::FaceMeshComponentName(TEXT("Face"));
FName ACharacterCreatorViewportHelper::BodyMeshComponentName(TEXT("Body"));
FName ACharacterCreatorViewportHelper::BottomMeshComponentName(TEXT("Bottom"));
FName ACharacterCreatorViewportHelper::ShoesMeshComponentName(TEXT("Shoes"));
FName ACharacterCreatorViewportHelper::HairGroomComponentName(TEXT("Hair"));
FName ACharacterCreatorViewportHelper::EyebrowsGroomComponentName(TEXT("Eyebrows"));
FName ACharacterCreatorViewportHelper::EyelashesGroomComponentName(TEXT("Eyelashes"));
FName ACharacterCreatorViewportHelper::MoustacheGroomComponentName(TEXT("Moustache"));
FName ACharacterCreatorViewportHelper::BeardGroomComponentName(TEXT("Beard"));

ACharacterCreatorViewportHelper::ACharacterCreatorViewportHelper()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(ACharacterCreatorViewportHelper::MeshComponentName);
	SkeletalComp->ComponentTags.Add("RootSkeletalMesh");
	SkeletalComp->ComponentTags.Add("Top");

	RootComponent = SkeletalComp;

	FaceMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacterCreatorViewportHelper::FaceMeshComponentName);
	FaceMesh->ComponentTags.Add(ACharacterCreatorViewportHelper::FaceMeshComponentName);
	FaceMesh->SetupAttachment(SkeletalComp);
	FaceMesh->SetForcedLOD(0);

	BodyMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacterCreatorViewportHelper::BodyMeshComponentName);
	BodyMesh->ComponentTags.Add(ACharacterCreatorViewportHelper::BodyMeshComponentName);
	BodyMesh->SetupAttachment(SkeletalComp);
	BodyMesh->SetForcedLOD(0);

	BottomMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacterCreatorViewportHelper::BottomMeshComponentName);
	BottomMesh->ComponentTags.Add(ACharacterCreatorViewportHelper::BottomMeshComponentName);
	BottomMesh->SetupAttachment(SkeletalComp);
	BottomMesh->SetForcedLOD(0);

	ShoesMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacterCreatorViewportHelper::ShoesMeshComponentName);
	ShoesMesh->ComponentTags.Add(ACharacterCreatorViewportHelper::ShoesMeshComponentName);
	ShoesMesh->SetupAttachment(SkeletalComp);
	ShoesMesh->SetForcedLOD(0);

	HairGroom = CreateOptionalDefaultSubobject<UGroomComponent>(ACharacterCreatorViewportHelper::HairGroomComponentName);
	HairGroom->ComponentTags.Add(ACharacterCreatorViewportHelper::HairGroomComponentName);
	HairGroom->SetupAttachment(GetFaceMesh());
	HairGroom->SetForcedLOD(0);

	EyebrowsGroom = CreateOptionalDefaultSubobject<UGroomComponent>(ACharacterCreatorViewportHelper::EyebrowsGroomComponentName);
	EyebrowsGroom->ComponentTags.Add(ACharacterCreatorViewportHelper::EyebrowsGroomComponentName);
	EyebrowsGroom->SetupAttachment(GetFaceMesh());
	EyebrowsGroom->SetForcedLOD(0);

	EyelashesGroom = CreateOptionalDefaultSubobject<UGroomComponent>(ACharacterCreatorViewportHelper::EyelashesGroomComponentName);
	EyelashesGroom->ComponentTags.Add(ACharacterCreatorViewportHelper::EyelashesGroomComponentName);
	EyelashesGroom->SetupAttachment(GetFaceMesh());
	EyelashesGroom->SetForcedLOD(0);

	MoustacheGroom = CreateOptionalDefaultSubobject<UGroomComponent>(ACharacterCreatorViewportHelper::MoustacheGroomComponentName);
	MoustacheGroom->ComponentTags.Add(ACharacterCreatorViewportHelper::MoustacheGroomComponentName);
	MoustacheGroom->SetupAttachment(GetFaceMesh());
	MoustacheGroom->SetForcedLOD(0);

	BeardGroom = CreateOptionalDefaultSubobject<UGroomComponent>(ACharacterCreatorViewportHelper::BeardGroomComponentName);
	BeardGroom->ComponentTags.Add(ACharacterCreatorViewportHelper::BeardGroomComponentName);
	BeardGroom->SetupAttachment(GetFaceMesh());
	BeardGroom->SetForcedLOD(0);

	CharacterCreatorComp = CreateDefaultSubobject<UCharacterCreatorComponent>(TEXT("CharacterCreatorComp"));
	CharacterCreatorComp->SetRootSkeletalMesh(SkeletalComp);
}
