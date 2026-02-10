#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
	CleanUp();
}

void Sphere::BeginPlay()
{
	//pRenderer_->Create(2,1,1,2);
	pRenderer_->Setting(2, 1, E_SAMPLER_PRESET::LINEAR_CLAMP, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_BLEND_PRESET::ALPHA_BLEND, E_RASTERIZER_PRESET::SOLID);
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Sphere::Tick(double deltaTime)
{
}

void Sphere::Render()
{
	pRenderer_->Render();
}

void Sphere::CleanUp()
{
}
