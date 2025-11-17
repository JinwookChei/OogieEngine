#pragma once



class SamplerManager
{
private:
	friend class Engine;
	SamplerManager();
	~SamplerManager();

public:
	static SamplerManager* Instance();

	//void Setting(const E_SAMPLER_TYPE& samplerMode);

	void Setting(unsigned int slot, const E_SAMPLER_TYPE& samplerMode);

private:
	void CleanUp();

	ISamplerState* pCurSampler_;
};
