#pragma once

enum class E_SAMPLER_MODE
{
	LINEAR_CLAMP = 0,
	LINEAR_WARP,
	ANISOTROPIC_CLAMP,
	ANISOTROPIC_WARP
};

class SamplerManager
{
private:
	friend class Engine;
	SamplerManager();
	~SamplerManager();

public:
	static SamplerManager* Instance();

	void Setting(const E_SAMPLER_MODE& samplerMode);

private:
	void CleanUp();

	ISamplerState** ppCurSampler_;

	ISamplerState* pLinearClamp_;
	ISamplerState* pLinearWarp_;
	ISamplerState* pAnisotropicClamp_;
	ISamplerState* pAnisotropicWarp_;
};
