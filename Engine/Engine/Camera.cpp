#include "stdafx.h"
#include "Transform.h"
#include "Camera.h"

Camera::Camera()
	: view_(),
	projection_(),
	fov_(60.0f),
	width_(800.0f),
	height_(600.0f),
	near_(0.01f),
	far_(100.0f),
	cameraSensitivity_(0.1f),
	cameraSpeed_(2.0f)
{

}

Camera::~Camera()
{
	CleanUp();
}

void Camera::Tick(double deltaTime)
{
	if (InputManager::Instance()->IsPress(VK_RBUTTON))
	{
		const Vector& deltaMouseMove = InputManager::Instance()->GetDeltaMouseMove();

		//pTransform_->RotateAroundAxis(pTransform_->UpVector(), deltaMouseMove.X * cameraSensitivity_);
		//pTransform_->RotateAroundAxis(pTransform_->RightVector(), deltaMouseMove.Y * cameraSensitivity_);
	}

	if (InputManager::Instance()->IsPress('W'))
	{
		DirectX::XMFLOAT4 forwardVector;
		XMStoreFloat4(&forwardVector, pTransform_->ForwardVector());
		Vector offset = { forwardVector.x, forwardVector.y , forwardVector.z };
		pTransform_->AddPosition(offset * cameraSpeed_ * (float)deltaTime);

	}
	if (InputManager::Instance()->IsPress('S'))
	{
		DirectX::XMFLOAT4 forwardVector;
		XMStoreFloat4(&forwardVector, pTransform_->ForwardVector());
		Vector offset = { forwardVector.x, forwardVector.y, forwardVector.z };
		pTransform_->AddPosition(-offset * cameraSpeed_ * (float)deltaTime);
	}
	if (InputManager::Instance()->IsPress('A'))
	{
		DirectX::XMFLOAT4 rightVector;
		XMStoreFloat4(&rightVector, pTransform_->RightVector());
		Vector offset = { rightVector.x , rightVector.y, rightVector.z };
		pTransform_->AddPosition(-offset * cameraSpeed_ * (float)deltaTime);
	}
	if (InputManager::Instance()->IsPress('D'))
	{
		DirectX::XMFLOAT4 rightVector;
		XMStoreFloat4(&rightVector, pTransform_->RightVector());
		Vector offset = { rightVector.x , rightVector.y , rightVector.z };
		pTransform_->AddPosition(offset * cameraSpeed_ * (float)deltaTime);
	}



	CameraTransformUpdate();
}

void Camera::BeginPlay()
{
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ -7.0f, 0.0f, 5.0f, 1.0f });

}

const DirectX::XMMATRIX& Camera::View() const
{
	return view_;
}

const DirectX::XMMATRIX& Camera::Projection() const
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
	DirectX::XMVECTOR eyePos = worldForm.GetPosition();
	DirectX::XMVECTOR eyeDir = worldForm.ForwardVector();
	DirectX::XMVECTOR eyeUp = worldForm.UpVector();

	view_ = DirectX::XMMatrixLookToLH(eyePos, eyeDir, eyeUp);
	float degToRad = PI / 180;

	projection_ = DirectX::XMMatrixPerspectiveFovLH(fov_ * degToRad, (width_ / height_), near_, far_);
}
