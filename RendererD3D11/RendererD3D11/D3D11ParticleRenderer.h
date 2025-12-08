#pragma once

struct CBParticle
{
	DirectX::XMFLOAT4X4 viewProj_;
	DirectX::XMFLOAT3 cameraRight_;
	float startSize_;
	DirectX::XMFLOAT3 cameraUp_;
	float endSize_;
	DirectX::XMFLOAT4 startColor_;
	DirectX::XMFLOAT4 endColor_;
};

struct ParticleGPU
{
	DirectX::XMFLOAT3 position_;
	float age_;
	DirectX::XMFLOAT3 velocity_;
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

	void __stdcall OnRender(const DirectX::XMMATRIX& viewProj, const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraUp) override;

	bool Init(ID3D11Device* pDevice, unsigned int maxParticleCnt, ID3D11ShaderResourceView* pTextureSRV);

	void Tick(ID3D11DeviceContext* pDeviceContext, float deltaTime);

	void Render(ID3D11DeviceContext* pDeviceContext, const DirectX::XMMATRIX& viewProj, const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraUp);

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