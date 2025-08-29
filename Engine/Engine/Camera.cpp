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
	far_(100.0f)
{

}

Camera::~Camera()
{
	CleanUp();
}

void Camera::Tick(double deltaTime)
{
	if (InputManager::Instance()->IsPress(VK_RBUTTON)) {
	}

	if (InputManager::Instance()->IsPress('W')) {
		pTransform_->AddPositionX(10.0f * deltaTime);
	}
	if (InputManager::Instance()->IsPress('S')) {
		pTransform_->AddPositionX(-10.0f * deltaTime);
	}
	if (InputManager::Instance()->IsPress('A')) {
		pTransform_->AddPositionY(-10.0f * deltaTime);
	}
	if (InputManager::Instance()->IsPress('D')) {
		pTransform_->AddPositionY(10.0f * deltaTime);
	}



	CameraTransformUpdate();
}

void Camera::BeginPlay()
{
	pTransform_->SetScale({1.0f, 1.0f, 1.0f, 0.0f});
	pTransform_->SetRotation({0.0f, 30.0f, 0.0f, 0.0f});
	pTransform_->SetPosition({-7.0f, 0.0f, 5.0f, 1.0f});
	
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

	projection_ = DirectX::XMMatrixPerspectiveFovLH(fov_ * degToRad, (width_/ height_), near_, far_);
}
