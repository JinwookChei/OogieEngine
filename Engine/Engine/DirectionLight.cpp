#include "stdafx.h"
#include "DirectionLight.h"

DirectionLight::DirectionLight()
{
}

DirectionLight::~DirectionLight()
{
	CleanUp();
}

void DirectionLight::Tick(double deltaTime)
{
	Light::Tick(deltaTime);
}

void DirectionLight::BeginPlay()
{
	Light::BeginPlay();
}

LightRenderData DirectionLight::GetData()
{
	LightRenderData data = {};
	data.lightDiffuse = DiffuseColor();
	data.lightSpecular = SpecularColor();
	data.lightAmbient = AmbientColor();
	data.direction_D_S = Direction();
	data.lightType = (float)E_LIGHT_TYPE::DIRECTION_LIGHT;
	data.lightIntensity = intensity_;
	return data;
}

Float3 DirectionLight::Direction() const
{
	Float4 forwardVector = pTransform_->ForwardVector();

	return Float3(forwardVector.X, forwardVector.Y, forwardVector.Z);
}

void DirectionLight::CleanUp()
{
}

int __stdcall DirectionLight::GetLightTypeForEditor() const
{
	return (int)E_LIGHT_TYPE::DIRECTION_LIGHT;
}

