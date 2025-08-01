// Created by Jorge Puerto. All Rights Reserved.

#pragma once

struct FExtractOptions
{
	bool bSpawnOneActorPerComponent = false;
	bool bUseLevelNameAsRootFolder = true;
	bool bForceStatic = false;
	bool bIterateAllActors = true;
	bool bSkipActorReplacementConfirmation = true;
	bool bOnlyConstructorComponentsActors = true;
	bool bDeleteOriginal = true;
	bool bSkipDeleteConfirmation = true;
	bool bReplaceAlreadyExtracted = false;
};