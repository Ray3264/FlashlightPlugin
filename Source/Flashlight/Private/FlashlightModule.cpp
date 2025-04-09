#include "FlashlightModule.h"
#include "FlashlightLog.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "SceneViewExtension.h"

#define LOCTEXT_NAMESPACE "FFlashlightModule"

void FFlashlightModule::StartupModule()
{
	UE_LOG(Flashlight, Log, TEXT("FFlashlightModule startup"));

	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("Flashlight"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Flashlight"), PluginShaderDir);

	FCoreDelegates::OnPostEngineInit.AddLambda([this]() {
		ViewExtension = FSceneViewExtensions::NewExtension<FFlashlightSceneViewExtension>();
		});
}

void FFlashlightModule::ShutdownModule()
{
	UE_LOG(Flashlight, Log, TEXT("FFlashlightModule shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFlashlightModule, Flashlight);