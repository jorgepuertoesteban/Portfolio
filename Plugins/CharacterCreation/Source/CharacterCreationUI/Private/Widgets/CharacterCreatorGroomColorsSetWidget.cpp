// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorGroomColorsSetWidget.h"
#include "Widgets/CharacterCreatorColorWidget.h"
#include "CharacterCreatorColorsSet.h"
#include "Components/PanelWidget.h"
#include "CharacterCreatorVectorMatAttribute.h"

bool UCharacterCreatorGroomColorsSetWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("UCharacterCreatorGroomColorsSetWidget::Initialize() Super::Initialize failed"));
		return false;
	}
	if (!VectorMatAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("VectorMatAttribute is null"));
		return false;
	}
	if (!PanelWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("PanelWidget is null"));
		return false;
	}

	RefreshWidget();

	return true;
}

void UCharacterCreatorGroomColorsSetWidget::SetColorsSet(const UCharacterCreatorColorsSet* NewColorsSet)
{
	ColorsSet = NewColorsSet;
	RefreshWidget();
}

void UCharacterCreatorGroomColorsSetWidget::SetVectorMatAttribute(const UCharacterCreatorVectorMatAttribute* NewVectorMatAttribute)
{
	VectorMatAttribute = NewVectorMatAttribute;
	RefreshWidget();
}

void UCharacterCreatorGroomColorsSetWidget::RefreshWidget()
{
	if (!ColorsSet)
	{
		return;
	}

	PanelWidget->ClearChildren();

	for (FLinearColor Color : ColorsSet->Colors)
	{
		UCharacterCreatorColorWidget* Widget = CreateWidget<UCharacterCreatorColorWidget>(this, ColorWidgetClass);
		if (Widget)
		{
			Widget->SetVectorMatAttribute(VectorMatAttribute);
			Widget->SetColor(Color);
			PanelWidget->AddChild(Widget);
			Widget->OnColorSelectedEvent.AddDynamic(this, &UCharacterCreatorGroomColorsSetWidget::ColorSelected);
		}
	}
}