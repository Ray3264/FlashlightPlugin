#include "FlashlightShaders.h"

IMPLEMENT_GLOBAL_SHADER(FFlashlightVS, "/Flashlight/Private/FlashlightShader.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FFlashlightPS, "/Flashlight/Private/FlashlightShader.usf", "MainPS", SF_Pixel);