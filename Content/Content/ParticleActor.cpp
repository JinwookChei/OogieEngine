#include "stdafx.h"
#include "ParticleActor.h"

ParticleActor::ParticleActor()
	// : pParticle_(new ParticleComponent(this))
{
	pParticle_ = CreateComponent<ParticleComponent>();
}

ParticleActor::~ParticleActor()
{
	CleanUp();
}

void ParticleActor::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
	//pParticle_->Tick(deltaTime);
}

void ParticleActor::BeginPlay()
{
}

void ParticleActor::Render()
{
	Actor::Render();
}

void ParticleActor::ParticleRender()
{
	pParticle_->RenderParticle();
}

void ParticleActor::CleanUp()
{
}
