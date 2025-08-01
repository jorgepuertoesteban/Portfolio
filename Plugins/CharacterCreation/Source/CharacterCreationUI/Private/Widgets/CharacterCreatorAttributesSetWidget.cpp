// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorAttributesSetWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "CharacterCreatorAttributesSet.h"
#include "Widgets/CharacterCreatorAttributeWidget.h"
#include "Components/PanelWidget.h"

bool UCharacterCreatorAttributesSetWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("UCategoryTabContentWidget::Initialize() Super::Initialize failed"));
		return false;
	}

	if (!PanelWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HorizontalBox is null"));
		return false;
	}

	RefreshWidget();

	return true;
}

void UCharacterCreatorAttributesSetWidget::SetAttributesSet(const UCharacterCreatorAttributesSet* NewAttributesSet)
{
	AttributesSet = NewAttributesSet;
	RefreshWidget();
}

void UCharacterCreatorAttributesSetWidget::RefreshWidget()
{
	if (!AttributesSet)
	{
		return;
	}

	for (const UCharacterCreatorAttribute* Attribute : AttributesSet->Attributes)
	{
		UCharacterCreatorAttributeWidget* Widget = CreateWidget<UCharacterCreatorAttributeWidget>(this, AttributeWidgetClass);
		if (Widget)
		{
			Widget->SetAttribute(Attribute);
			PanelWidget->AddChild(Widget);
		}
	}
}