// Juan Hernandez-Vargas
// Estudio2 Games.

#pragma once

#include "Modules/ModuleManager.h"

class FSkaroUtilsModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// Centralized access to utilities.
	void RegisterAllTools();
};
