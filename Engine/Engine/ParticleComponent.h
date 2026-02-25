#pragma once

class ParticleTTTTT;
class ParticleComponent
{
public:
	ENGINE_API ParticleComponent(Actor* pOwner);

	ENGINE_API ~ParticleComponent();

	ENGINE_API void Tick(double deltaTime);

	ENGINE_API void Render();

private:
	void CleanUp();

	Actor* pOwner_;
	
	uint32_t particleNum_;
	double accTime_;
	uint32_t particleType_;

	float startSize_;
	float endSize_;
	Float4 startColor_;
	Float4 endColor_;


	IPSO* pPSO_;
	IMesh* pParticle_;
};
