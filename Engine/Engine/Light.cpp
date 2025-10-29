#include "stdafx.h"
#include "Light.h"

Light::Light()
	: diffuseColor_(1.0f, 1.0f, 1.0f, 1.0f),
	specularColor_(1.0f, 1.0f, 1.0f, 1.0f),
	ambientColor_(0.2f, 0.2f, 0.2f, 1.0f)
{
}

Light::~Light()
{
	CleanUp();
}

const Color& Light::DiffuseColor() const
{
	return diffuseColor_;
}

const Color& Light::SpecularColor() const
{
	return specularColor_;
}

const Color& Light::AmbientColor() const
{
	return ambientColor_;
}

void Light::CleanUp()
{
}
