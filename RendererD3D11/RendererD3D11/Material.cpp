#include "stdafx.h"
#include "Material.h"


Material::Material()
	: refCount_(1),
	shineness_(1.0f),
	specularColor_(0.7f, 0.7f, 0.7f)
{
}

Material::~Material()
{
	CleanUp();
}

bool Material::Init(float shineness,Float3 specularColor)
{
	shineness_ = shineness;
	specularColor_ = specularColor;
	
	return true;
}

HRESULT __stdcall Material::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Material::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Material::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall Material::Setting()
{

}

float Material::GetShineness() const
{
	return shineness_;
}

void Material::SetShineness(float shineness)
{
	shineness_ = shineness;
}

const Float3& Material::GetSpecularColor() const
{
	return specularColor_;
}

void Material::SetSpecularColor(const Float3& specularColor)
{
	specularColor_ = specularColor;
}

void Material::CleanUp()
{
}
