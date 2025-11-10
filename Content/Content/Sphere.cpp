#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere()
	:pRenderer_(new RenderComponent(this))
{
}

Sphere::~Sphere()
{
	CleanUp();
}

void Sphere::BeginPlay()
{
	pRenderer_->Create(MESH_TYPE::SPHERE);
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
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
