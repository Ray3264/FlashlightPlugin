#pragma once
#include "CoreMinimal.h"
#include "ScreenPass.h"
#include "SceneTexturesConfig.h"


class FFlashlightVS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FFlashlightVS);

	FFlashlightVS() = default;
	FFlashlightVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) {}
};

class FFlashlightPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFlashlightPS);
	SHADER_USE_PARAMETER_STRUCT(FFlashlightPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		RENDER_TARGET_BINDING_SLOTS()
		SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
		SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneTextureUniformParameters, SceneTexturesStruct)
		SHADER_PARAMETER(float, Strenght)
		SHADER_PARAMETER(float, Brightness)
		SHADER_PARAMETER(float, Size)
		SHADER_PARAMETER(FVector3f, Color)
		SHADER_PARAMETER(float, Distance)
		SHADER_PARAMETER(int32, UseDepth)
		SHADER_PARAMETER(int32, UseTexture)
		SHADER_PARAMETER(int32, UseBlendFix)
	END_SHADER_PARAMETER_STRUCT()
};