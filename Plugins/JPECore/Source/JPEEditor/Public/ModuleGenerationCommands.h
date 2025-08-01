// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 4
#include "Styling/SlateStyle.h"
#include "EditorStyleSet.h"
#include "JPEEditorStyle.h"
#elif ENGINE_MAJOR_VERSION == 5
#include "Styling/AppStyle.h"
#endif

class FModuleGenerationCommands : public TCommands<FModuleGenerationCommands>
{
public:


    FModuleGenerationCommands()
        : TCommands<FModuleGenerationCommands>(
            TEXT("ModuleGeneration"), 
            NSLOCTEXT("Contexts", "ModuleGenerationCommands", "Module Generation Commands"), 
            NAME_None, 
#if ENGINE_MAJOR_VERSION == 4
            FJPEEditorStyle::GetAppStyleSetName()
#elif ENGINE_MAJOR_VERSION == 5
            FAppStyle::GetAppStyleSetName()
#endif
            )
    {}
    void RegisterCommands() override;
    
public:

    TSharedPtr<FUICommandInfo> NewModule;
    
};

