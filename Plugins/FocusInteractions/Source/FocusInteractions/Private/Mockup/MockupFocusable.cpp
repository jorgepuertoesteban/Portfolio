// Created by Jorge Puerto. All Rights Reserved.


#include "Mockup/MockupFocusable.h"

#include "Components/FocusableComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

#include "ActionableType.h"

AMockupFocusable::AMockupFocusable() :Super()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> StaticMeshAsset;
		FConstructorStatics()
			: StaticMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetStaticMesh(ConstructorStatics.StaticMeshAsset.Get());

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(GetRootComponent());
	TextRenderComponent->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);

	FocusableComponent = CreateDefaultSubobject<UFocusableComponent>(TEXT("FocusableComponent"));
}

#if WITH_EDITOR
void AMockupFocusable::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (FocusableComponent && FocusableComponent->ActionableType)
	{
		TextRenderComponent->SetText(FText::FromName(FocusableComponent->ActionableType->GetActionFriendlyName()));
	}
	else
	{
		TextRenderComponent->SetText(FText::FromString(TEXT("No Action Selected")));
	}

}
#endif // WITH_EDITOR