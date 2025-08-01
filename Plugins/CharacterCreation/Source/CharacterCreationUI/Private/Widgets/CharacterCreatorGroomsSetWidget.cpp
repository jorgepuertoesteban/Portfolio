// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorGroomsSetWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "CharacterCreatorGroomsSet.h"
#include "Widgets/CharacterCreatorGroomWidget.h"
#include "Components/PanelWidget.h"

bool UCharacterCreatorGroomsSetWidget::Initialize()
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

void UCharacterCreatorGroomsSetWidget::SetGroomsSet(const UCharacterCreatorGroomsSet* NewGroomsSet)
{
	GroomsSet = NewGroomsSet;
	RefreshWidget();
}

void UCharacterCreatorGroomsSetWidget::RefreshWidget()
{
	if (!GroomsSet)
	{
		return;
	}

	for (const UCharacterCreatorGroom* Groom : GroomsSet->Grooms)
	{
		UCharacterCreatorGroomWidget* Widget = CreateWidget<UCharacterCreatorGroomWidget>(this, GroomWidgetClass);
		if (Widget)
		{
			Widget->SetGroom(Groom);
			PanelWidget->AddChild(Widget);
			Widget->OnGroomSelectedEvent.AddDynamic(this, &UCharacterCreatorGroomsSetWidget::GroomSelected);
		}
	}
}