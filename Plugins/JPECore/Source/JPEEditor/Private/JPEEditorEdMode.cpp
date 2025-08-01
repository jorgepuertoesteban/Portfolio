// Created by Jorge Puerto. All Rights Reserved.

#include "JPEEditorEdMode.h"
#include "JPEEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FJPEEditorEdMode::EM_JPEEditorEdModeId = TEXT("EM_JPEEditorEdMode");

FJPEEditorEdMode::FJPEEditorEdMode()
{

}

FJPEEditorEdMode::~FJPEEditorEdMode()
{

}

void FJPEEditorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FJPEEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FJPEEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FJPEEditorEdMode::UsesToolkits() const
{
	return true;
}




