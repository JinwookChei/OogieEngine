#include "stdafx.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(Actor* pOwner)
	: pOwner_(pOwner)
	, pPSO_(nullptr)
{
}

MeshComponent::~MeshComponent()
{
	CleanUp();
}

void MeshComponent::Tick(double deltaTime)
{

}

void MeshComponent::Render()
{
	ObjectFrameData objectFrameData;
	objectFrameData.worldMatrix = pOwner_->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}

void MeshComponent::InitPSO
(
	uint16_t meshSlotCount,
	uint16_t meshMaterialCount,
	E_DEPTH_PRESET depthState,
	E_RASTERIZER_PRESET rasterizerMode
)
{	
	if (nullptr != pPSO_)
	{
		pPSO_->Release();
		pPSO_ = nullptr;
	}

	PipelineStateDesc psoDesc;
	psoDesc.meshSlotCount = meshSlotCount;
	psoDesc.materialSlotCount = meshMaterialCount;
	psoDesc.depthState = depthState;
	psoDesc.rasterizerMode = rasterizerMode;

	pPSO_ = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
	if (nullptr == pPSO_)
	{
		DEBUG_BREAK();
	}
}

IPSO* MeshComponent::GetPSO() const
{
	return pPSO_;
}

void MeshComponent::CleanUp()
{
	if (nullptr != pPSO_)
	{
		pPSO_->Release();
		pPSO_ = nullptr;
	}
}

