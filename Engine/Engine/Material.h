#pragma once

class Material
{
public:
	Material();

	Material(IShader* pVertexShader, IShader* pPixelShader, ISamplerState* pSampleState);

	virtual ~Material();

	IShader* GetVertexShader() const;

	void SetSampler(ISamplerState* sampler, uint32_t slot = 0);

	void Setting();
	
private:
	void CleanUp();

	IShader* pVertexShaderImpl_;

	IShader* pPixelShaderImpl_;

	ISamplerState* pSampleStateImpl_;
};
