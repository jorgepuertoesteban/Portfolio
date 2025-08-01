// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class AActor;
class UWorld;
class UClass;

class FJPEEditAction_MeshFromBlueprint
{
public:

	static const TSet<FString> IgnoreClassNames;
	static const TSet<UClass*> IgnoreClasses;

	static void ExecuteAction();
	
	static void SpawnActorsFromComponents(AActor* Actor, UWorld* World, UClass* ChosenClass);

	static bool IgnoreActor(AActor* Actor);
};