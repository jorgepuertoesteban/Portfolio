// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "JPEEditorStatics.h"
#include "Editor/EditorEngine.h"
#include "Engine.h"

UWorld* UJPEEditorStatics::GetEditorMainWorld()
{
	return GEditor->GetEditorWorldContext().World();
}
