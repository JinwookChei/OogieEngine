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
}

void DirectionLight::BeginPlay()
{
}

void DirectionLight::BindLight()
{
	CBPerLight cbPerLight;
	cbPerLight.lightDiffuse = DiffuseColor();
	cbPerLight.lightSpecular = SpecularColor();
	cbPerLight.lightAmbient = AmbientColor();
	cbPerLight.direction_D_S = Direction();
	cbPerLight.lightType = (float)ELightType::DirectionLight;
	ConstantManager::Instance()->UpdatePerLight(&cbPerLight);
}

Float3 DirectionLight::Direction() const
{
	Float4 forwardVector = pTransform_->ForwardVector();

	return Float3(forwardVector.X, forwardVector.Y, forwardVector.Z);
}

void DirectionLight::CleanUp()
{
}
