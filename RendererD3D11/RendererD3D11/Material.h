#pragma once

class Shader;
class Texture;

class Material : public IMaterial
{
private:
	friend class RenderResourceFactory;

	Material();
	virtual ~Material();

	static IMaterial* Create(const MaterialDesc& desc);
	bool Init(const MaterialDesc& desc);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

	void __stdcall SetTextures(unsigned int texIdx, ITexture* pTexture) override;

	float GetShineness() const override;

	void SetShineness(float shineness) override;

	const Float3& GetSpecularColor() const override;

	void SetSpecularColor(const Float3& specularColor) override;

	const E_SAMPLER_PRESET& GetSamplerState() const;

	const E_BLEND_PRESET& GetBlendState() const;

	void Bind();

private:
	void CleanTextures();
	void CleanUp();

	ULONG refCount_;

	E_SHADER_PRESET shaderType_;
	E_SAMPLER_PRESET samplerState_;
	E_BLEND_PRESET blendState_;

	ULONG texturesNum_;
	Texture** ppTextures_;

	// 얘네도 필요 없음.
	float shineness_;
	Float3 specularColor_;
};
