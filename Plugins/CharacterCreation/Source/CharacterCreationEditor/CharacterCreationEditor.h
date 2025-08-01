// Created by Jorge Puerto. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class ICharacterCreationEditorModule : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static ICharacterCreationEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked<ICharacterCreationEditorModule>("CharacterCreationEditor");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static bool IsAvailable() 
	{
		return FModuleManager::Get().IsModuleLoaded("CharacterCreationEditor");
	}
	
	virtual TSharedPtr<class FUICommandList> GetPluginCommands() = 0;

	virtual EAssetTypeCategories::Type GetAssetCategoryBit() const = 0;
	
};