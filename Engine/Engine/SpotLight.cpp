#include "stdafx.h"
#include "Light.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: range_(30.0f),
	angle_(cosf(MATH::ConvertDegToRad(30.0f)))
{
	
}

SpotLight::~SpotLight()
{
	CleanUp();
}

void SpotLight::Tick(double deltaTime)
{

}

void SpotLight::BeginPlay()
{
}

void SpotLight::BindLight()
{
	CBPerLight cbPerLight;
	cbPerLight.lightDiffuse = DiffuseColor();
	cbPerLight.lightSpecular = SpecularColor();
	cbPerLight.lightAmbient = AmbientColor();
	cbPerLight.direction_D_S = Direction();
	const Float4& worldPos = GetWorldTransform().GetPosition();
	cbPerLight.position_S_P = { worldPos.X,worldPos.Y, worldPos.Z };
	cbPerLight.angle_S = angle_;
	cbPerLight.range_S_P = range_;
	cbPerLight.lightType = (float)ELightType::SpotLight;

	ConstantManager::Instance()->UpdatePerLight(&cbPerLight);
}

Float3 SpotLight::Direction() const
{
	Float4 forwardVector = pTransform_->ForwardVector();

	return Float3(forwardVector.X, forwardVector.Y, forwardVector.Z);
}

float SpotLight::GetRange() const
{
	return range_;
}

void SpotLight::SetRange(float range)
{
	range_ = range;
}

float SpotLight::GetAngle() const
{
	return angle_;
}

void SpotLight::SetAngle(float deg)
{
	angle_ = cosf(MATH::ConvertDegToRad(deg));
}

void SpotLight::CleanUp()
{
}
