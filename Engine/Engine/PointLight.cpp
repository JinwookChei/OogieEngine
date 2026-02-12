#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight()
	:range_(3.0f),
	attenuationConst_(1.0f),
	attenuationLinear_(0.09f),
	attenuationQuad_(0.032f)
{
}

PointLight::~PointLight()
{
	CleanUp();
}

void PointLight::Tick(double deltaTime)
{
	Light::Tick(deltaTime);
}

void PointLight::BeginPlay()
{
	Light::BeginPlay();
}

LightRenderData PointLight::GetData()
{
	LightRenderData data = {};
	data.lightDiffuse = DiffuseColor();
	data.lightSpecular = SpecularColor();
	data.lightAmbient = AmbientColor();
	data.range_S_P = range_;
	const Float4& worldPos = GetWorldTransform().GetPosition();
	data.position_S_P = { worldPos.X, worldPos.Y, worldPos.Z };
	data.attenuationConst_S_P = attenuationConst_;
	data.attenuationLinear_S_P = attenuationLinear_;
	data.attenuationQuad_S_P = attenuationQuad_;
	data.lightType = (float)ELightType::PointLight;
	return data;
}

float PointLight::GetRange() const
{
	return range_;
}

void PointLight::SetRange(float range)
{
	range_ = range;
}

void PointLight::CleanUp()
{
}
