#include "stdafx.h"
#include "DirectionLight.h"

DirectionLight::DirectionLight()
{
}

DirectionLight::~DirectionLight()
{
}

void DirectionLight::Tick(double deltaTime)
{
}

void DirectionLight::BeginPlay()
{
}

Float3 DirectionLight::Direction() const
{
	Float4x4 tmp = pTransform_->GetWorldMatrix();
	pTransform_->GetPosition();
	return Float3();
}

void DirectionLight::CleanUp()
{
}
