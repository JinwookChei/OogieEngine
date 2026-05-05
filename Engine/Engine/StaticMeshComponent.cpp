#include "stdafx.h"
#include "MeshComponent.h"
#include "StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::~StaticMeshComponent()
{
	CleanUp();
}

void StaticMeshComponent::BeginPlay()
{
}

void StaticMeshComponent::Tick(double deltaTime)
{
	MeshComponent::Tick(deltaTime);
}

void StaticMeshComponent::Render(bool isFirst)
{
	MeshComponent::Render(isFirst);

	if (isFirst)
	{
		pPSO_->GetMaterial(0);
		pPSO_->SetDepthState(E_DEPTH_PRESET::DEPTH_ENABLE_WRITE);
	}
	else
	{
		pPSO_->SetDepthState(E_DEPTH_PRESET::DEPTH_ENABLE_READ_ONLY);
	}

	ObjectFrameData objectFrameData;
	MATH::MatrixMultiply(objectFrameData.worldMatrix, GetComponentTransform().GetMatrix(), GetOwner()->GetWorldTransform().GetMatrix());
	objectFrameData.scale = GetWorldScale();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_, isFirst);
}

void StaticMeshComponent::CleanUp()
{

}
