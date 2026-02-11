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

//void DirectionLight::Render()
//{
//	Renderer::Instance()->UpdateLightFrame(GetData());
//}

LightRenderData DirectionLight::GetData()
{
	LightRenderData data = {};
	data.lightDiffuse = DiffuseColor();
	data.lightSpecular = SpecularColor();
	data.lightAmbient = AmbientColor();
	data.direction_D_S = Direction();
	data.lightType = (float)ELightType::DirectionLight;
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
