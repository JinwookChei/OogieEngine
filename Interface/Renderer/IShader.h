#pragma once

enum class E_SHADER_PRESET
{
	Light = 0,
	ParticleCompute,
	ParticleRender,
	StaticMesh,
	SkinnedMesh
};


struct IShader : public IUnknown
{

};