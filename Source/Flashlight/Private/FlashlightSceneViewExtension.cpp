#include "FlashlightSceneViewExtension.h"
#include "FlashlightShaders.h"

#include "FXRenderingUtils.h"
#include "PostProcess/PostProcessInputs.h"
#include "DynamicResolutionState.h"


static TAutoConsoleVariable<int32> CVarEnabled(
	TEXT("r.FSP"),
	1,
	TEXT("Controls FSP plugin\n")
	TEXT(" 0: disabled\n")
	TEXT(" 1: enabled (default)"));

static TAutoConsoleVariable<float> CVarEffectStrenght(
	TEXT("r.FSP.EffectStrenght"),
	0.1f,
	TEXT("Controls an amount of depth buffer blending to the base color."));

static TAutoConsoleVariable<float> CVarSize(
	TEXT("r.Flashlight.Size"),
	420.0f,
	TEXT("Flashlight size in pixels"));

static TAutoConsoleVariable<float> CVarBrightness(
	TEXT("r.Flashlight.Brightness"),
	10.0f,
	TEXT("Flashlight brightness"));

static TAutoConsoleVariable<float> CVarDistance(
	TEXT("r.Flashlight.Distance"),
	0.1f,
	TEXT("Flashlight distance"));

FFlashlightSceneViewExtension::FFlashlightSceneViewExtension(const FAutoRegister& AutoRegister) :
	FSceneViewExtensionBase(AutoRegister)
{

}

void FFlashlightSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	if (CVarEnabled->GetInt() == 0)
	{
		return;
	}

	Inputs.Validate();

	const FIntRect PrimaryViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);

	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

	if (!SceneColor.IsValid())
	{
		return;
	}

	const FScreenPassTextureViewport Viewport(SceneColor);

	FRDGTextureDesc SceneColorDesc = SceneColor.Texture->Desc;

	SceneColorDesc.Format = PF_FloatRGBA;
	FLinearColor ClearColor(0., 0., 0., 0.);
	SceneColorDesc.ClearValue = FClearValueBinding(ClearColor);

	FRDGTexture* ResultTexture = GraphBuilder.CreateTexture(SceneColorDesc, TEXT("FlashlightResult"));
	FScreenPassRenderTarget ResultRenderTarget = FScreenPassRenderTarget(ResultTexture, SceneColor.ViewRect, ERenderTargetLoadAction::EClear);

	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	TShaderMapRef<FFlashlightVS> ScreenPassVS(GlobalShaderMap);
	TShaderMapRef<FFlashlightPS> ScreenPassPS(GlobalShaderMap);

	FFlashlightPS::FParameters* Parameters = GraphBuilder.AllocParameters<FFlashlightPS::FParameters>();
	Parameters->View = View.ViewUniformBuffer;
	Parameters->SceneTexturesStruct = Inputs.SceneTextures;
	Parameters->Strenght = CVarEffectStrenght->GetFloat();
	Parameters->Brightness = CVarBrightness->GetFloat();
	Parameters->Size = CVarSize->GetFloat();
	Parameters->Color = FVector3f(1.0f, 0.9f, 0.78f);
	Parameters->Distance = CVarDistance->GetFloat();
	Parameters->UseDepth = 1;
	Parameters->UseTexture = 1;
	Parameters->UseBlendFix = 1;

	Parameters->RenderTargets[0] = ResultRenderTarget.GetRenderTargetBinding();

	AddDrawScreenPass(
		GraphBuilder,
		RDG_EVENT_NAME("FlashlightShader"),
		View,
		Viewport,
		Viewport,
		ScreenPassVS,
		ScreenPassPS,
		Parameters
	);

	AddCopyTexturePass(GraphBuilder, ResultTexture, SceneColor.Texture);
}
