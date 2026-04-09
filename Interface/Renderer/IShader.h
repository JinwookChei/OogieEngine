#pragma once

enum class E_SHADER_PRESET
{
	LIGHT = 0,
	PARTICLE_COMPUTE,
	PARTICLE_RENDER,
	TEST_MESH,
	SIMPLE_MESH,
	STATIC_MESH,
	SKINNED_MESH,
	DEBUG_LINE,
	BLIT
};


struct IShader : public IUnknown
{

};