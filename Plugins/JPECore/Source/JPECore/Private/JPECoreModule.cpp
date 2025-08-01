// Created by Jorge Puerto. All Rights Reserved.

#include "JPECoreModule.h"

#define LOCTEXT_NAMESPACE "FJPECoreModule"

DEFINE_LOG_CATEGORY(LogJPECore);

void FJPECoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FJPECoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJPECoreModule, JPECore)