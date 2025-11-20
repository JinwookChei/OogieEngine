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

	MaterialDesc matDesc;
	matDesc.VS = L"ObjectVS.cso";
	matDesc.PS = L"DeferredObjectPS.cso";
	matDesc.shineness = 0.7f;
	matDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	pMaterial_ = GRenderer->CreateMaterial(matDesc);

	const wchar_t* texPath = L"../Resource/Texture/Bricks_4K/Bricks_Color.png";
	pTextureColor_ = GRenderer->LoadTextureFromDirectXTex(texPath, false);
	texPath = L"../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
	pTextureNormal_ = GRenderer->LoadTextureFromDirectXTex(texPath, true);

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