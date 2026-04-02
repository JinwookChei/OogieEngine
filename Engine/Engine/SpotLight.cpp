#include "stdafx.h"
#include "StaticMeshComponent.h"
#include "Light.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: range_(30.0f),
	smooth_(32.0f),
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
	pStaticMesh_->GetComponentTransform().SetRotation({ 0.0f, -90.0f, 0.0f, 0.0f });
}

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
	data.smooth_S = smooth_;
	data.innerAngle_S = innerAngle_;
	data.outerAngle_S = outerAngle_;
	data.attenuationConst_S_P = attenuationConst_;
	data.attenuationLinear_S_P = attenuationLinear_;
	data.attenuationQuad_S_P = attenuationQuad_;
	data.lightType = (float)E_LIGHT_TYPE::SPOT_LIGHT;
	data.lightIntensity = intensity_;

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

void SpotLight::CleanUp()
{
}

int __stdcall SpotLight::GetLightTypeForEditor() const
{
	return (int)E_LIGHT_TYPE::SPOT_LIGHT;
}

float __stdcall SpotLight::GetLightRange() const
{
	return range_;
}

float __stdcall SpotLight::GetLightSmooth() const
{
	return smooth_;
}

float __stdcall SpotLight::GetLightInnerAngle() const
{
	return innerAngle_;
}

float __stdcall SpotLight::GetLightOuterAngle() const
{
	return outerAngle_;
}

void __stdcall SpotLight::SetLightRange(float range)
{
	SetRange(range);
}

void __stdcall SpotLight::SetLightSmooth(float smooth)
{
	smooth_ = smooth;
}

void __stdcall SpotLight::SetLightInnerAngle(float innerAngle)
{
	innerAngle_ = innerAngle;
}

void __stdcall SpotLight::SetLightOuterAngle(float outerAngle)
{
	outerAngle_ = outerAngle;
}
