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
	pRenderer_->Create(2,1,1,2);
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
