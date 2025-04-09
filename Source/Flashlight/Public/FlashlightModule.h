#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "FlashlightSceneViewExtension.h"

class FFlashlightModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	TSharedPtr< class FFlashlightSceneViewExtension, ESPMode::ThreadSafe > ViewExtension;
};