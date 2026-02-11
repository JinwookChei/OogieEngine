#include "stdafx.h"
#include "Light.h"
#include "RenderComponent.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: range_(30.0f),
	exponent_(32.0f),
	innerAngle_(15.0f),
	outerAngle_(30.0f),
	attenuationConst_(1.0f),
	attenuationLinear_(0.09f),
	attenuationQuad_(0.032f)
{
	
}

SpotLight::~SpotLight()
{
	CleanUp();
}

void SpotLight::Tick(double deltaTime)
{
	Light::Tick(deltaTime);
}

void SpotLight::BeginPlay()
{
	Light::BeginPlay();
}

//void SpotLight::Render()
//{
//	Renderer::Instance()->UpdateLightFrame(GetData());
//}

LightRenderData SpotLight::GetData()
{
	LightRenderData data = {};
	data.lightDiffuse = DiffuseColor();
	data.lightSpecular = SpecularColor();
	data.lightAmbient = AmbientColor();
	data.direction_D_S = Direction();
	data.range_S_P = range_;
	const Float4& worldPos = GetWorldTransform().GetPosition();
	data.position_S_P = { worldPos.X,worldPos.Y, worldPos.Z };
	data.exponent_S = exponent_;
	data.innerAngle_S = innerAngle_;
	data.outerAngle_S = outerAngle_;
	data.attenuationConst_S_P = attenuationConst_;
	data.attenuationLinear_S_P = attenuationLinear_;
	data.attenuationQuad_S_P = attenuationQuad_;
	data.lightType = (float)ELightType::SpotLight;

	return data;
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

//float SpotLight::GetAngle() const
//{
//	return angle_;
//}

//void SpotLight::SetAngle(float deg)
//{
//	angle_ = cosf(MATH::ConvertDegToRad(deg));
//}

void SpotLight::CleanUp()
{
}
