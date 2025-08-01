// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExtractOptions.h"

class AActor;
class UWorld;
class UClass;


class FJPEEditActionsLib
{

public:

	static FName ExtractedTag;
	static FString LevelFolderPrefix;

public:
	
	static bool SpawnActorsFromComponents(AActor* Actor, UWorld* World, UClass* ChosenClass, const FExtractOptions& Options);
	static void CreateFolderPath(AActor* Actor, UClass* ChosenClass, bool const bUseLevelNameAsRootFolder = true, AActor* OriginalActor = nullptr);
	static bool HasConstructorComponents(const AActor* Actor);
	static bool IsAnExtractedActor(const AActor* Actor);
};