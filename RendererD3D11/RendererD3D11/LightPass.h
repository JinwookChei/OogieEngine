#pragma once



struct CBPerLight
{
	Color lightDiffuse;
	Color lightSpecular;
	Color lightAmbient;

	Float3 direction_D_S; // Spot, DirectionÀÌ¶û °øÀ¯.
	float range_S_P;
	Float3 position_S_P;

	float exponent_S;
	float innerAngle_S;
	float outerAngle_S;

	float attenuationConst_S_P;
	float attenuationLinear_S_P;
	float attenuationQuad_S_P;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	float lightType;

	Float2 pad;
};


class LightPass final
	: public IUnknown
{
	friend class Renderer;

	LightPass();

	~LightPass();

	bool Init();

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void RenderBegin(IRenderTarget* pGBufferTarget);

	void Render(const LightRenderData& lightData);

	void RenderEnd(IRenderTarget* pGBufferTarget);

private:
	bool InitScreenQuad();

	bool InitShaders();

	bool InitConstantBuffer();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();

	ULONG refCount_;

	Shader* pShader_;

	ConstantBuffer* pCBPerLight_;
	//ConstantBuffer* pCBPerObject_;
	Mesh* pScreenQuad_;

	SamplerState* pSamplerState_;
	BlendState* pBlendState_;
};
