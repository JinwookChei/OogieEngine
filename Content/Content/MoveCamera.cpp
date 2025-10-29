#include "stdafx.h"
#include "MoveCamera.h"

MoveCamera::MoveCamera()
	:pRenderer_(new RenderComponent(this))
{
}

MoveCamera::~MoveCamera()
{
	CleanUp();
}

void MoveCamera::Tick(double deltaTime)
{
	if (InputManager::Instance()->IsPress(VK_RBUTTON))
	{
		const Float2& deltaMouseMove = InputManager::Instance()->GetDeltaMouseMove();

		/*DirectX::XMVECTOR a = pTransform_->UpVector();

		DirectX::XMVECTOR b = pTransform_->RightVector();

		pTransform_->RotateAroundAxis(pTransform_->UpVector(), deltaMouseMove.X * cameraSensitivity_);

		pTransform_->RotateAroundAxis(pTransform_->RightVector(), deltaMouseMove.Y * cameraSensitivity_);*/
	}

	if (InputManager::Instance()->IsPress('W'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();

		Float4 offset;

		MATH::VectorScale(offset, forwardVector, cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('S'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();

		Float4 offset;

		MATH::VectorScale(offset, forwardVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('A'))
	{
		Float4 rightVector = pTransform_->RightVector();

		Float4 offset;

		MATH::VectorScale(offset, rightVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('D'))
	{
		Float4 rightVector = pTransform_->RightVector();

		Float4 offset;

		MATH::VectorScale(offset, rightVector, cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('F'))
	{
		Float4 upVector = pTransform_->UpVector();

		Float4 offset;

		MATH::VectorScale(offset, upVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		//pTransform_->AddPosition(offset);
		pTransform_->AddRotaionY(-1.0f);
	}
	if (InputManager::Instance()->IsPress('R'))
	{
		Float4 upVector = pTransform_->UpVector();

		Float4 offset;

		MATH::VectorScale(offset, upVector, cameraSpeed_ * (float)deltaTime);

		//pTransform_->AddPosition(offset);
		pTransform_->AddRotaionY(1.0f);
	}
	if (InputManager::Instance()->IsPress('Q'))
	{
		pTransform_->AddRotaionZ(-50 * deltaTime);
	}
	if (InputManager::Instance()->IsPress('E'))
	{
		pTransform_->AddRotaionZ(50 * deltaTime);
	}
}

void MoveCamera::BeginPlay()
{
	pRenderer_->Create(MESH_TYPE::CUBE);

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
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
