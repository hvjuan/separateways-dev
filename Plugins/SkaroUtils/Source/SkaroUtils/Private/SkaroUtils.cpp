// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkaroUtils.h"

#define LOCTEXT_NAMESPACE "FSkaroUtilsModule"

void FSkaroUtilsModule::StartupModule()
{
	UE_LOG(LogTemp, Display, TEXT("Skaro Utils module loaded"));
}

void FSkaroUtilsModule::ShutdownModule()
{
	UE_LOG(LogTemp, Display, TEXT("Skaro Utils module unloaded"));
}

void FSkaroUtilsModule::RegisterAllTools()
{
	UE_LOG(LogTemp, Display, TEXT("Registering all tools"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSkaroUtilsModule, SkaroUtils)