#pragma once

class Shader;
class ConstantBuffer;
class SamplerState;
class BlendStateT;

struct CBPerComputeParticle
{
	float deltaTime_;
	unsigned int maxParticleNum_;
	float accTime_;
	unsigned int patternType_;
};

struct CBPerParticle
{
	Float4x4 world_;
	Float4x4 viewProj_;
	Float3 cameraRight_;
	float startSize_;
	Float3 cameraUp_;
	float endSize_;
	Float4 startColor_;
	Float4 endColor_;
};



class ParticlePass final : public IUnknown/*: public IParticleRenderer*/
{
	friend class Renderer;
	ParticlePass();
	~ParticlePass();

	bool Init();

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Tick(IParticle* pParticle, float deltaTime);

	void Render(IParticle* pParticle, const Float4x4 worldTransform, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp);

private:

	bool InitShaders();

	bool InitConstantBuffer();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();
	
	ULONG refCount_;

	Shader* pComputeShader_;
	Shader* pRenderShader_;

	ConstantBuffer* pCBPerComputeParticle;
	ConstantBuffer* pCBPerParticle_;

	SamplerState* pSamplerState_;
	BlendState* pBlendState_;
};