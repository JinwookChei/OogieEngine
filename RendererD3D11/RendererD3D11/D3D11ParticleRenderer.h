#pragma once

struct CBParticle
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

struct CBParticleCompute
{
	float deltaTime_;
	unsigned int maxParticleNum_;
	float accTime_;
	unsigned int patternType_;
};

class ParticleSystemGPU final : public IParticleRenderer
{
public:

	ParticleSystemGPU();
	virtual ~ParticleSystemGPU();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall OnTick(IParticle* pParticle, double deltaTime) override;

	void __stdcall OnRender(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp) override;

	bool Init();

	void Tick(IParticle* pParticle, float deltaTime);

	void Render(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp);

private:

	bool InitShaders();

	bool InitConstantBuffer();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();
	
	ULONG refCount_;


	ID3D11VertexShader* pVertexShader_;
	ID3D11GeometryShader* pGeometryShader_;
	ID3D11PixelShader* pPixelShader_;
	ID3D11ComputeShader* pComputeShader_;


	ID3D11Buffer* pConstantBuffer_;
	ID3D11Buffer* pComputeConstantBuffer_;


	ID3D11BlendState* pBlendState_;
	ID3D11SamplerState* pSamplerState_;
};