#pragma once

struct MaterialDesc
{
	E_SHADER_PRESET shaderType;
	ULONG textureNum_ = 0;
	float shineness = 16.0f;
	Float3 specularColor = { 0.7f,0.7f ,0.7f };
};

struct IMaterial : public IUnknown
{
	virtual void __stdcall SetTextures(unsigned int texIdx, ITexture* pTexture) = 0;

	virtual float __stdcall GetShineness() const = 0;

	virtual void __stdcall SetShineness(float shineness) = 0;

	virtual const Float3& __stdcall GetSpecularColor() const = 0;

	virtual void __stdcall SetSpecularColor(const Float3& specularColor) = 0;
};