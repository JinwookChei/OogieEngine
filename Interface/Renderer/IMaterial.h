#pragma once

enum class E_SAMPLER_PRESET
{
	LINEAR_CLAMP = 0,
	LINEAR_WARP,
	ANISOTROPIC_CLAMP,
	ANISOTROPIC_WARP,
	DISABLE
};

enum class E_BLEND_PRESET
{
	OPAQUE_BLEND = 0,				// 블렌딩 없음 (불투명)
	ALPHA_BLEND,					// 일반 알파 블렌딩
	ADDITIVE_BLEND,					// Additive(가산) 블렌딩
	DISABLE
};

struct MaterialDesc
{
	E_SHADER_PRESET shaderType;
	E_SAMPLER_PRESET samplerState;
	E_BLEND_PRESET blendState;
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