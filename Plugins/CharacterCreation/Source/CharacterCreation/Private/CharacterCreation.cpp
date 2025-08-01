// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreation.h"

#define LOCTEXT_NAMESPACE "FCharacterCreationModule"

int32 FCharacterCreationModule::CharacterCreationDebugVar = 0;
FAutoConsoleVariableRef ConVarDebugCharacterCreation(TEXT("dis.debug-charactercreation"), FCharacterCreationModule::CharacterCreationDebugVar, TEXT("Debug CharacterCreation"), ECVF_Cheat);


void FCharacterCreationModule::StartupModule()
{
		
}

void FCharacterCreationModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterCreationModule, CharacterCreation)