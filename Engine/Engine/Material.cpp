#include "stdafx.h"
#include "Material.h"

Material::Material()
	:pVertexShader_(nullptr),
	pPixelShader_(nullptr),
	pMaterial_(nullptr){
}

Material::~Material()
{
	if (nullptr != pVertexShader_)
	{
		pVertexShader_->Release();
		pVertexShader_ = nullptr;
	}
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->Release();
		pPixelShader_ = nullptr;
	}
	if (nullptr != pMaterial_)
	{
		pMaterial_->Release();
		pMaterial_ = nullptr;
		pMaterial_ = nullptr;
	}
}
