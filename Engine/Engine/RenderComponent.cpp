#include "stdafx.h"
#include "Actor.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(Actor* pOwner)
	: pOwner_(pOwner)
	, pPSO_(nullptr)
	, pMesh_(nullptr)
	, pMaterial_(nullptr)
	, OnMeshLoaded_()
{
}

RenderComponent::~RenderComponent()
{
	CleanUp();
}

void RenderComponent::Render()
{
	ObjectFrameData objectFrameData;
	objectFrameData.world = pOwner_->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->RenderTest(pPSO_);
}

void RenderComponent::Setting
(
	uint16_t meshTag, 
	uint16_t materialTag, 
	E_DEPTH_PRESET depthState, 
	E_RASTERIZER_PRESET rasterizerMode
)
{
	CleanUp();

	IMesh* tmpMesh = nullptr;
	if (false == MeshManager::Instance()->GetMesh(&tmpMesh, meshTag)) DEBUG_BREAK();
	pMesh_ = tmpMesh;
	pMesh_->AddRef();

	IMaterial* tmpMaterial = nullptr;
	if (false == MaterialManager::Instance()->GetMaterial(&tmpMaterial, materialTag)) DEBUG_BREAK();
	pMaterial_ = tmpMaterial;
	pMaterial_->AddRef();

	PipelineStateDesc psoDesc;
	psoDesc.pMesh = pMesh_;
	psoDesc.pMaterial = pMaterial_;
	psoDesc.depthState = depthState;
	psoDesc.rasterizerMode = rasterizerMode;

	pPSO_ = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
	if (nullptr == pPSO_)
	{
		DEBUG_BREAK();
	}
}

IMesh* RenderComponent::GetMesh() const
{
	return pMesh_;
}

void RenderComponent::BindOnMeshLoaded(MeshLoadedDelegate callback)
{
	if (!callback)
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

	if (nullptr != pPSO_)
	{
		pPSO_->Release();
		pPSO_ = nullptr;
	}
}