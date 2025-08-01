// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class FBaseEditorToolCustomization : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();

	//IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	//End of IDetailCustomization interface

	static FReply ExecuteToolCommand(IDetailLayoutBuilder* DetailBuilder, UFunction* MethodToExecute);

};
