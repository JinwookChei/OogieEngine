#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight()
	:range_(10.0f),
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
}

void PointLight::BeginPlay()
{
}

void PointLight::BindLight()
{
	CBPerLight cbPerLight;
	cbPerLight.lightDiffuse = DiffuseColor();
	cbPerLight.lightSpecular = SpecularColor();
	cbPerLight.lightAmbient = AmbientColor();
	const Float4& worldPos = GetWorldTransform().GetPosition();
	cbPerLight.position_S_P = { worldPos.X, worldPos.Y, worldPos.Z };
	cbPerLight.range_S_P = range_;
	cbPerLight.attenuationConst_P = attenuationConst_;
	cbPerLight.attenuationLinear_P = attenuationLinear_;
	cbPerLight.attenuationQuad_P = attenuationQuad_;
	
	cbPerLight.lightType = (float)ELightType::PointLight;
	ConstantManager::Instance()->UpdatePerLight(&cbPerLight);
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
