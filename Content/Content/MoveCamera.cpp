#include "stdafx.h"
#include "MoveCamera.h"

MoveCamera::MoveCamera()
{
}

MoveCamera::~MoveCamera()
{
	CleanUp();
}

void MoveCamera::Tick(double deltaTime)
{
	if (InputManager::IsPress(VK_RBUTTON))
	{
		const Float2& deltaMouseMove = InputManager::GetDeltaMouseMove();

		pTransform_->AddRotaionZ(deltaMouseMove.X * deltaTime * 8.0f);

		pTransform_->AddRotaionY(deltaMouseMove.Y * deltaTime * 8.0f);
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

void MoveCamera::BeginPlay()
{
	pRenderer_->Create(E_MESH_TYPE::CUBE);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ -10.0f, 0.0f, 0.0f, 1.0f });
}

void MoveCamera::Render()
{
	pRenderer_->Render();
}

void MoveCamera::CleanUp()
{

}
