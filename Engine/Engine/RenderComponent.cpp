#include "stdafx.h"
#include "Actor.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(Actor* pOwner)
	: pOwner_(pOwner),
	pMesh_(nullptr),
	pMaterial_(nullptr),
	pTextureColor_(nullptr),
	pTextureNormal_(nullptr),
	pInputLayout_(nullptr),
	OnMeshLoaded_()
{
}

RenderComponent::~RenderComponent()
{
	CleanUp();
}

void RenderComponent::Render()
{
	CBPerObject cbPerObject;
	cbPerObject.world = pOwner_->GetWorldTransform().GetWorldMatrixTranspose();
	cbPerObject.materialSpecular = pMaterial_->GetSpecularColor();
	cbPerObject.materialShineness = pMaterial_->GetShineness();

	ConstantManager::Instance()->UpdatePerObejct(&cbPerObject);

	pMaterial_->Setting();
	pMesh_->Setting();
	pTextureColor_->Setting(0);
	pTextureNormal_->Setting(1);
	pInputLayout_->Setting();
	//pRasterizer_->Setting();
	pMesh_->Draw();
}


void RenderComponent::Create(E_MESH_TYPE meshType)
{
	CleanUp();

	switch (meshType)
	{
	case E_MESH_TYPE::SPHERE:
		if (!MeshManager::Instance()->GetMesh(&pMesh_, 2))
		{
			DEBUG_BREAK();
			return;
		}
		break;
	case E_MESH_TYPE::CUBE:
		if (!MeshManager::Instance()->GetMesh(&pMesh_, 1))
		{
			DEBUG_BREAK();
			return;
		}
		break;
	default:
		break;
	}
	pMesh_->AddRef();

	if (!MaterialManager::Instance()->GetMaterial(&pMaterial_, 1))
	{
		DEBUG_BREAK();
		return;
	}
	pMaterial_->AddRef();


	if (!TextureManager::Instance()->GetTexture(&pTextureColor_, 1))
	{
		DEBUG_BREAK();
		return;
	}
	pTextureColor_->AddRef();

	if (!TextureManager::Instance()->GetTexture(&pTextureNormal_, 2))
	{
		DEBUG_BREAK();
		return;
	}
	pTextureNormal_->AddRef();

	pInputLayout_ = GRenderer->CreateLayout(pMesh_, pMaterial_->GetVertexShader());

	BroadcastOnMeshLoaded();
}

IMesh* RenderComponent::GetMesh() const
{
	return pMesh_;
}

void RenderComponent::BindOnMeshLoaded(MeshLoadedDelegate callback)
{
	if(!callback)
	{
		DEBUG_BREAK();
		return;
	}
	OnMeshLoaded_.push_back(callback);
}

void RenderComponent::BroadcastOnMeshLoaded()
{
	for (const auto& callback : OnMeshLoaded_)
	{
		if (!callback)
		{
			DEBUG_BREAK();
			continue;
		}
		callback(pMesh_);
	}
}

void RenderComponent::CleanUp()
{
	if (nullptr != pTextureColor_)
	{
		pTextureColor_->Release();
		pTextureColor_ = nullptr;
	}

	if (nullptr != pTextureNormal_)
	{
		pTextureNormal_->Release();
		pTextureNormal_ = nullptr;
	}

	if (nullptr != pMesh_)
	{
		pMesh_->Release();
		pMesh_ = nullptr;
	}

	if (nullptr != pMaterial_)
	{
		pMaterial_->Release();
		pMaterial_ = nullptr;
	}

	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
}