#pragma once

enum class E_PARTICLE_PATTERN_TYPE
{
	EXPLOSION = 0,
	FUME
};

struct ParticleDesc
{
	unsigned int maxNum_;

	E_PARTICLE_PATTERN_TYPE patternType_;

	ITexture* pTexture_;
};

struct IParticle : public IUnknown
{

};
