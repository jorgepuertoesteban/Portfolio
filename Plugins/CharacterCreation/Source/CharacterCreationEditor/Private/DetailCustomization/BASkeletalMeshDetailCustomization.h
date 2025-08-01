// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

class FBASkeletalMeshDetailCustomization : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();

	//IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	//End of IDetailCustomization interface

	static FReply ExecuteOnClick(IDetailLayoutBuilder* DetailBuilder);
};
