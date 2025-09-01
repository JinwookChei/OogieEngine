#include "stdafx.h"
#include "Light.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: spotPosition_({ 0.0f, 0.0f, 2.0f }),
	spotDirection_({ 0.0f, 0.0f, -1.0f }),
	spotRange_(5.0f),
	spotAngle_(cosf(ConvertDegToRad(20.0f)))
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

const Float3& SpotLight::SpotPosition() const
{
	return spotPosition_;
}

const Float3& SpotLight::SpotDirection() const
{
	return spotDirection_;
}

float SpotLight::SpotRange() const
{
	return spotRange_;
}

float SpotLight::SpotAngle() const
{
	return spotAngle_;
}

void SpotLight::CleanUp()
{
}
