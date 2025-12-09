#pragma once

class Texture;


struct ParticleElem
{
	Float3 position_;
	float age_;
	Float3 velocity_;
	float lifeTime_;
};

class Particle : public IParticle
{
private:
	friend class Renderer;

	Particle();
	virtual ~Particle();
	static IParticle* Create(const ParticleDesc& desc);
	bool Init(const ParticleDesc& desc);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	E_PARTICLE_PATTERN_TYPE GetPatternType() const;

	unsigned int GetMaxNumber() const;

	double GetAccTime() const;

	void AddAccTime(double deltaTime);

	ID3D11ShaderResourceView* GetParticlesSRV() const;

	ID3D11UnorderedAccessView* GetParticlesUAV() const;

	Texture* GetTexture() const;

private:
	bool InitParticleBuffer(unsigned int maxNum);

	bool InitTexture(ITexture* pTexture);

	void CleanUp();

	ULONG refCount_;

	E_PARTICLE_PATTERN_TYPE patternType_;

	double accTime_;

	unsigned int maxNum_;

	ID3D11Buffer* pParticles_;
	ID3D11ShaderResourceView* pParticlesSRV_;
	ID3D11UnorderedAccessView* pParticlesUAV_;

	Texture* pTexture_;
};

