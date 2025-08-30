#include "stdafx.h"
#include "Light.h"

Light::Light()
	:lightColor_({1, 1, 1, 1}),
	ambientColor_({ 0.2f, 0.2f, 0.2f, 0.2f })
{
}

Light::~Light()
{
	CleanUp();
}

const DirectX::XMFLOAT4& Light::LightColor() const
{
	return lightColor_;
}

const DirectX::XMFLOAT4& Light::AmbientColor() const
{
	return ambientColor_;
}

void Light::CleanUp()
{
}
