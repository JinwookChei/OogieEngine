#include "stdafx.h"
#include "Transform.h"
#include "Camera.h"

Camera::Camera()
	: fov_(60.0f),
	width_(800.0f),
	height_(600.0f),
	near_(0.01f),
	far_(100.0f),
	cameraSensitivity_(10.0f),
	cameraSpeed_(2.0f)
{
	MatrixIdentity(view_);
	MatrixIdentity(projection_);
}

Camera::~Camera()
{
	CleanUp();
}

void Camera::Tick(double deltaTime)
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

		VectorScale(offset, forwardVector, cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('S'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();

		Float4 offset;

		VectorScale(offset, forwardVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('A'))
	{
		Float4 rightVector = pTransform_->RightVector();

		Float4 offset;

		VectorScale(offset, rightVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('D'))
	{
		Float4 rightVector = pTransform_->RightVector();

		Float4 offset;

		VectorScale(offset, rightVector, cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('Q'))
	{
		Float4 upVector = pTransform_->UpVector();

		Float4 offset;

		VectorScale(offset, upVector, -1.0f * cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
	if (InputManager::Instance()->IsPress('E'))
	{
		Float4 upVector = pTransform_->UpVector();

		Float4 offset;

		VectorScale(offset, upVector, cameraSpeed_ * (float)deltaTime);

		pTransform_->AddPosition(offset);
	}
}

void Camera::BeginPlay()
{
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ -10.0f, 0.0f, 0.0f, 1.0f });

}

void Camera::Render()
{
	CameraTransformUpdate();
}

const Float4x4& Camera::View() const
{
	return view_;
}

const Float4x4& Camera::Projection() const
{
	return projection_;
}

void Camera::SetWidth(float width)
{
	width_ = width;
}

void Camera::SetHeight(float height)
{
	height_ = height;
}

void Camera::SetConfig(float fov, float Near, float Far)
{
	fov_ = fov;
	near_ = Near;
	far_ = Far;
}

void Camera::CleanUp()
{
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
}

void Camera::CameraTransformUpdate()
{
	const Transform& worldForm = GetWorldTransform();
	Vector eyePos = worldForm.GetPosition();
	Vector eyeDir = worldForm.ForwardVector();
	Vector eyeUp = worldForm.UpVector();

	MatrixLookToLH(view_, eyePos, eyeDir, eyeUp);

	float fovRad = ConvertDegToRad(fov_);

	MatrixPerspectiveFovLH(projection_, fov_, (width_ / height_), near_, far_);	
}

