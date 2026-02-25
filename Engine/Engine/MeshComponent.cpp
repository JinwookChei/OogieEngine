#include "stdafx.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent()
	:  pPSO_(nullptr)
{
}

MeshComponent::~MeshComponent()
{
	CleanUp();
}

void MeshComponent::BeginPlay()
{

}

void MeshComponent::Tick(double deltaTime)
{
	SceneComponent::Tick(deltaTime);
}

void MeshComponent::Render()
{
	SceneComponent::Render();
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

