// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorColorWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "CharacterCreatorGroom.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


bool UCharacterCreatorColorWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize() Super::Initialize failed"));
		return false;
	}

	if (!SubmitButton)
	{
		UE_LOG(LogTemp, Error, TEXT("SubmitButton is null"));
		return false;
	}

	SubmitButton->OnPressed.AddDynamic(this, &UCharacterCreatorColorWidget::OnButtonPressed);

	RefreshWidget();

	return true;
}

void UCharacterCreatorColorWidget::SetColor(FLinearColor NewColor)
{
	Color = NewColor;
	RefreshWidget();
}

void UCharacterCreatorColorWidget::SetVectorMatAttribute(const UCharacterCreatorVectorMatAttribute* NewVectorMatAttribute)
{
	VectorMatAttribute = NewVectorMatAttribute;
	RefreshWidget();
}

void UCharacterCreatorColorWidget::RefreshWidget()
{

	if (IconImage)
	{
		IconImage->SetColorAndOpacity(Color);
	}

	FLinearColor NormalColor = Color;
	NormalColor.A = 1.f;
	FLinearColor HoveredColor = Color;
	HoveredColor.A = 0.5;

	FButtonStyle Style = SubmitButton->GetStyle();
	Style.Normal.TintColor = NormalColor;
	Style.Hovered.TintColor = HoveredColor;
	SubmitButton->SetStyle(Style);
}

void UCharacterCreatorColorWidget::OnButtonPressed()
{
	if (!VectorMatAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("UCharacterCreatorColorWidget::OnButtonPressed() No VectorMatAttribute Found"));
		return;
	}
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCharacterCreatorColorWidget::OnButtonPressed() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCControlComp->Server_SetVectorMaterialAttributeValue(CCControlComp->MainCharacterCreator, VectorMatAttribute, Color);
	OnColorSelectedEvent.Broadcast();
}

bool UCharacterCreatorColorWidget::IsListItemSelectable() const
{
	return true;
}

