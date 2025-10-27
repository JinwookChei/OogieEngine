#include "stdafx.h"
#include "Light.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: spotPosition_({ -1.5f, 0.0f, 0.0f }),
	spotDirection_({ 1.0f, 0.0f, 0.0f }),
	spotRange_(30.0f),
	spotAngle_(cosf(ConvertDegToRad(15.0f)))
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
