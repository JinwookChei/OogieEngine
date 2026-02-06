#pragma once

enum class E_SAMPLER_PRESET
{
	LINEAR_CLAMP = 0,
	LINEAR_WARP,
	ANISOTROPIC_CLAMP,
	ANISOTROPIC_WARP
};

enum class E_DEPTH_PRESET
{
	DEPTH_ENABLE_WRITE = 0,
	DEPTH_ENABLE_READ_ONLY,
	DEPTH_DISABLE
};

enum class E_BLEND_PRESET
{
	OPAQUE_BLEND = 0,				// 블렌딩 없음 (불투명)
	ALPHA_BLEND,					// 일반 알파 블렌딩
	ADDITIVE_BLEND,					// Additive(가산) 블렌딩
};

// Pipeline State Object
struct IPSO : IUnknown
{

};