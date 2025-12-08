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

struct ParticleGPU
{
	Float3 position_;
	float age_;
	Float3 velocity_;
	float lifeTime_;
};

struct CBParticleCompute
{
	float deltaTime_;
	unsigned int maxParticles_;
	float time_;
	unsigned int spawnMode_;
};

class ParticleSystemGPU final : public IParticleRenderer
{
public:
	float gTimeAcc_;
	int gGpuPatternMode_;

	ParticleSystemGPU();
	~ParticleSystemGPU();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall OnTick(double deltaTime) override;

	void __stdcall OnRender(const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp) override;

	bool Init(ID3D11Device* pDevice, unsigned int maxParticleCnt, ID3D11ShaderResourceView* pTextureSRV);

	void Tick(ID3D11DeviceContext* pDeviceContext, float deltaTime);

	void Render(ID3D11DeviceContext* pDeviceContext, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp);

private:
	bool InitParticleBuffer(ID3D11Device* pDevice, unsigned int maxParticleCnt);

	bool InitShaders(ID3D11Device* pDevice);

	bool InitConstantBuffer(ID3D11Device* pDevice);

	bool InitBlendState(ID3D11Device* pDevice);

	bool InitSamplerState(ID3D11Device* pDevice);

	void CleanUp();

	unsigned int maxParticleCnt_;

	ID3D11Buffer* pParticleBuffer_;
	ID3D11ShaderResourceView* pParticleSRV_;
	ID3D11UnorderedAccessView* pParticleUAV_;

	ID3D11VertexShader* pVertexShader_;
	ID3D11GeometryShader* pGeometryShader_;
	ID3D11PixelShader* pPixelShader_;
	ID3D11ComputeShader* pComputeShader_;

	ID3D11Buffer* pConstantBuffer_;
	ID3D11Buffer* pComputeConstantBuffer_;

	ID3D11BlendState* pBlendState_;
	ID3D11SamplerState* pSamplerState_;

	ID3D11ShaderResourceView* pParticleTextureSRV_;

	ULONG refCount_;
};