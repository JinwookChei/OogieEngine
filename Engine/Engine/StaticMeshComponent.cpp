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

void StaticMeshComponent::Render()
{
	MeshComponent::Render();

	ObjectFrameData objectFrameData;
	objectFrameData.worldMatrix = GetOwner()->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}

void StaticMeshComponent::CleanUp()
{

}
