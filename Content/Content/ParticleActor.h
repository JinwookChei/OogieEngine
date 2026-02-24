#pragma once

class ParticleActor : public Actor
{
public:
	ParticleActor();
	~ParticleActor();

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void Render() override;

	void ParticleRender() override;

private:
	void CleanUp() override;

	ParticleComponent* pParticle_;
};
