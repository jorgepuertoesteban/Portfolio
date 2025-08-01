// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreationBodyTypeWidget.h"

#include "CharacterCreationTypes.h"
#include "CharacterCreator.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UCharacterCreationBodyTypeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentOptionIndex = INDEX_NONE;
	OptionsSlider->SetMinValue(0);
	OptionsSlider->SetMaxValue(Options.Num() - 1);

	if (const UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer()))
	{
		const FCharacterCreationBodyType CurrentBodyType = CCControlComp->GetCharacterCreator()->GetSelectedBodyType();
		for (int32 i = 0; i < Options.Num(); ++i)
		{
			if (Options[i] != CurrentBodyType)
			{
				continue;
			}

			CurrentOptionIndex = i;
			OptionsSlider->SetValue(i);

			break;
		}
	}

	OptionsSlider->OnValueChanged.AddUniqueDynamic(this, &UCharacterCreationBodyTypeWidget::OnOptionsSliderValueChanged);
}

void UCharacterCreationBodyTypeWidget::OnOptionsSliderValueChanged(float NewValue)
{
	const int32 NewValueInt = FMath::RoundToInt32(NewValue);
	if (CurrentOptionIndex == NewValueInt)
	{
		return;
	}
	CurrentOptionIndex = NewValueInt;
	if (Options.IsValidIndex(NewValueInt))
	{
		if (UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer()))
		{
			CCControlComp->SetBodyType(CCControlComp->GetCharacterCreator(), Options[NewValueInt]);
		}
	}
}
