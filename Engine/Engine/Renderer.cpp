#include "stdafx.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"

Renderer::Renderer()
	:
	pMesh_(nullptr),
	pMaterial_(nullptr),
	pInputLayout_(nullptr)
{
}

Renderer::~Renderer()
{
	if (nullptr != pMesh_) 
	{
		delete pMesh_;
		pMesh_ = nullptr;
	}
	if (nullptr != pMaterial_)
	{
		delete pMaterial_;
		pMaterial_ = nullptr;
	}
	if (nullptr != pInputLayout_)
	{
		delete pInputLayout_;
		pInputLayout_ = nullptr;
	}
}


void Renderer::Setting()
{
	pMaterial_->pMaterial_->Setting();
	pInputLayout_->pInputLayout_->Setting();
	pMesh_->pVertex_->Setting();
}

void Renderer::Draw()
{
	pMesh_->pVertex_->Draw();
}