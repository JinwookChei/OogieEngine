#include "stdafx.h"
#include "ParticleActor.h"

ParticleActor::ParticleActor()
	: pParticle_(new ParticleComponent(this))
{
}

ParticleActor::~ParticleActor()
{
	CleanUp();
}

void ParticleActor::Tick(double deltaTime)
{
	pParticle_->Tick(deltaTime);
}

void ParticleActor::BeginPlay()
{
}

void ParticleActor::Render()
{
}

void ParticleActor::ParticleRender()
{
	pParticle_->Render();
}

void ParticleActor::CleanUp()
{
	if (nullptr != pParticle_)
	{
		delete pParticle_;
		pParticle_ = nullptr;
	}
}
