// Created by Jorge Puerto. All rights reserved.

#include "FocusInteractions.h"
#include "FocusInteractionsLog.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FFocusInteractionsModule"


DEFINE_LOG_CATEGORY(FocusInteractionsLog);

int32 FFocusInteractionsModule::DebugDrawingFTC = 0;
FAutoConsoleVariableRef ConVarDebugFocusTracer(TEXT("dis.debug-focustracer"), FFocusInteractionsModule::DebugDrawingFTC, TEXT("Draw debug for Focus Tracer"), ECVF_Cheat);

void FFocusInteractionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFocusInteractionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFocusInteractionsModule, FocusInteractions)