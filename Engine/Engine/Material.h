#pragma once

class Material
{
public:
	Material();

	Material(IMaterial* pMaterial);

	virtual ~Material();

	void SetVertexShader(IShader* vertexShader);

	void SetPixelShader(IShader* pixelShader);

	void SetSampler(ISamplerState* sampler, uint32_t slot = 0);

	void Setting();

	void SetMaterial(IMaterial* pMaterial);

private:
	void CleanUp();

	IMaterial* pMaterialImpl_;

};
