// Created by Bionic Ape. All Rights Reserved.


#include "UI/ActionSetSelectorWidget.h"
#include "ActionsSet.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"
#include "Slate/ButtonStyleAsset.h"


bool UActionSetSelectorWidget::Initialize()
{
	if (!Super::Initialize() || !ActionsSet || !Button || !NormalMaster || !HoveredMaster || !DisabledMaster || !PressedMaster || !NormalButtonStyleAsset)
	{
		return false;
	}

	NormalStyle = NormalButtonStyleAsset->ButtonStyle;


	NormalMID = UMaterialInstanceDynamic::Create(NormalMaster, this);
	NormalMID->SetTextureParameterValue(TEXT("IconTexture"), ActionsSet->GetIcon());
	NormalStyle.Normal.SetResourceObject(NormalMID);

	HoveredMID = UMaterialInstanceDynamic::Create(HoveredMaster, this);
	HoveredMID->SetTextureParameterValue(TEXT("IconTexture"), ActionsSet->GetIcon());
	NormalStyle.Hovered.SetResourceObject(HoveredMID);

	DisabledMID = UMaterialInstanceDynamic::Create(DisabledMaster, this);
	DisabledMID->SetTextureParameterValue(TEXT("IconTexture"), ActionsSet->GetIcon());
	NormalStyle.Disabled.SetResourceObject(DisabledMID);

	PressedMID = UMaterialInstanceDynamic::Create(PressedMaster, this);
	PressedMID->SetTextureParameterValue(TEXT("IconTexture"), ActionsSet->GetIcon());
	NormalStyle.Pressed.SetResourceObject(PressedMID);

	Button->SetStyle(bIsSelected ? SelectedStyle : NormalStyle);


	
	
	return true;
}
