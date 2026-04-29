#include "stdafx.h"
#include "RunTimeMode.h"
#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
	cameraSpeed_ = 4.0f;
}

EditorCamera::~EditorCamera()
{
	CleanUp();
}

void EditorCamera::Tick(double deltaTime)
{
	Camera::Tick(deltaTime);

	if (RunTimeMode::GetCurrentMode() != E_RUNTIME_MODE::EDITOR) return;
	if (InputManager::IsPress(VK_RBUTTON))
	{
		const Float2& deltaMouseMove = InputManager::GetDeltaMouseMove();
		pTransform_->AddRotaionZ(deltaMouseMove.X * deltaTime * 10.0f);
		pTransform_->AddRotaionY(deltaMouseMove.Y * deltaTime * 10.0f);
	}

	if (InputManager::IsPress('W'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('S'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, -1.0f * cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('A'))
	{
		Float4 rightVector = pTransform_->RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, -1.0f * cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('D'))
	{
		Float4 rightVector = pTransform_->RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('F'))
	{
		pTransform_->AddRotaionY(1.0f);
	}
	if (InputManager::IsPress('R'))
	{
		pTransform_->AddRotaionY(-1.0f);
	}
	if (InputManager::IsPress('Q'))
	{
		pTransform_->AddRotaionZ(-50 * deltaTime);
	}
	if (InputManager::IsPress('E'))
	{
		pTransform_->AddRotaionZ(50 * deltaTime);
	}
}

void EditorCamera::BeginPlay()
{
	/*IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 11);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 10);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);*/

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void EditorCamera::Render()
{
	Camera::Render();
}

void EditorCamera::CleanUp()
{
}
