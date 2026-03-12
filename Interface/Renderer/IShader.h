#pragma once

enum class E_SHADER_PRESET
{
	LIGHT = 0,
	PARTICLE_COMPUTE,
	PARTICLE_RENDER,
	STATIC_MESH,
	SKINNED_MESH,
	DEBUG_LINE
};


struct IShader : public IUnknown
{

};