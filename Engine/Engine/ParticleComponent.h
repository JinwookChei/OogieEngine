#pragma once

class ParticleTTTTT;
class ParticleComponent 
	: public SceneComponent
{
public:
	ENGINE_API ParticleComponent();

	ENGINE_API ~ParticleComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

	ENGINE_API void RenderParticle();

private:
	void CleanUp() override;
	
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
