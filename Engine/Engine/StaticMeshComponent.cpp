#include "stdafx.h"
#include "MeshComponent.h"
#include "StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent(Actor* pOwner)
	: MeshComponent(pOwner)
{
}

StaticMeshComponent::~StaticMeshComponent()
{
	CleanUp();
}

void StaticMeshComponent::Tick(double deltaTime)
{
}

void StaticMeshComponent::Render()
{
	ObjectFrameData objectFrameData;
	objectFrameData.worldMatrix = pOwner_->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}

void StaticMeshComponent::CleanUp()
{

}
