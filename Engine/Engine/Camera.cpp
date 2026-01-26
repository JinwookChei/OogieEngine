#include "stdafx.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "Camera.h"

Camera::Camera()
	: fov_(DEFAULT_FOV)
	, near_(DEFAULT_NEAR)
	, far_(DEFAULT_FAR)
	, cameraSensitivity_(10.0f)
	, cameraSpeed_(2.0f)
	, screenOffset_({ 0.0f, 0.0f })
	, screenScale_({ 1.0f, 1.0f })
	, pGBufferRenderTarget_(nullptr)
	, pFinalRenderTarget_(nullptr)
	, pDebugRenderTarget_(nullptr)
{
	if (nullptr == GMainCamera)
	{
		GMainCamera = this;
	}

	MATH::MatrixIdentity(view_);

	MATH::MatrixIdentity(projection_);

	InitGBuffer();

	InitLightBuffer();

	InitParticleBuffer();

	InitDebugBuffer();
}

Camera::~Camera()
{
	CleanUp();
}

void Camera::Tick(double deltaTime)
{
}

void Camera::BeginPlay()
{
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ -10.0f, 0.0f, 0.0f, 1.0f });
}

void Camera::Render()
{

}

void Camera::GeometryPassBegin()
{
	pGBufferRenderTarget_->Clear();
	pGBufferRenderTarget_->Bind();
}

void Camera::GeometryPassEnd()
{
	pGBufferRenderTarget_->EndRenderPass();
}

void Camera::LightPassBegin()
{
	pFinalRenderTarget_->Clear();
	pFinalRenderTarget_->Bind();
}
void Camera::LightPassEnd()
{
	pFinalRenderTarget_->EndRenderPass();
}

void Camera::ParticlePassBegin()
{
	pParticleRenderTarget_->Clear();
	pParticleRenderTarget_->Bind();
}

void Camera::ParticlePassEnd()
{
	pParticleRenderTarget_->EndRenderPass();
}

void Camera::BlitToBackBuffer()
{
	GRenderer->RenderFinal(pParticleRenderTarget_);
}

const Float4x4& Camera::View() const
{
	return view_;
}

const Float4x4& Camera::Projection() const
{
	return projection_;
}

void Camera::SetSize(const Float2& size)
{
	// 주의!! SetSize하는데 렌더 타겟이 없으면 크래쉬 발생유도.
	if (nullptr == pGBufferRenderTarget_)
	{
		Assert("pGBufferTarget_ == NULL!");
		return;
	}

	RenderTargetDesc desc = pGBufferRenderTarget_->GetDesc();
	pGBufferRenderTarget_->Release();
	pGBufferRenderTarget_ = nullptr;

	desc.size_ = size;
	pGBufferRenderTarget_ = GRenderer->CreateRenderTarget(desc);
}


void Camera::SetConfig(float fov, float Near, float Far)
{
	fov_ = fov;
	near_ = Near;
	far_ = Far;
}

float Camera::GetFov() const
{
	return fov_;
}

float Camera::GetNear() const
{
	return near_;
}

float Camera::GetFar() const
{
	return far_;
}

void Camera::SetClearColor(const Color& clearColor)
{
	pGBufferRenderTarget_->SetClearColor(clearColor);
}

void Camera::SetScreenPlacement(const Float2& screenOffset, const Float2& screenScale)
{
	screenOffset_ = screenOffset;
	screenScale_ = screenScale;
}

Float2 Camera::GetRenderSize() const
{
	return pGBufferRenderTarget_->GetSize();
}

IRenderTarget* Camera::GetGBufferTarget() const
{
	return pGBufferRenderTarget_;
}

void Camera::UpdatePerFrameConstant()
{
	CameraTransformUpdate();

	CameraFrameData frameData;
	frameData.view = view_;
	frameData.projection = projection_;
	frameData.screenOffset = screenOffset_;
	frameData.screenScale = screenScale_;
	frameData.screenResolution = pGBufferRenderTarget_->GetSize();
	frameData.camPos = pTransform_->GetPosition();

	GRenderer->UpdateCameraFrame(frameData);
}

bool Camera::InitGBuffer()
{
	RenderTargetDesc gBufferDesc(E_RENDER_TECHNIQUE_TYPE::Deferred);
	gBufferDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	gBufferDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	pGBufferRenderTarget_ = GRenderer->CreateRenderTarget(gBufferDesc);

	if (nullptr == pGBufferRenderTarget_)
	{
		return false;
	}

	return true;
}

bool Camera::InitLightBuffer()
{
	RenderTargetDesc lightBufferDesc(E_RENDER_TECHNIQUE_TYPE::Forward);
	lightBufferDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	lightBufferDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	lightBufferDesc.useDepthStencil_ = false;						// 라이트 패스에서 여러 라이트를 처리하기 위해서는 Depth는 꺼야함.
	pFinalRenderTarget_ = GRenderer->CreateRenderTarget(lightBufferDesc);
	if (nullptr == pFinalRenderTarget_)
	{
		return false;
	}

	return true;
}

bool Camera::InitParticleBuffer()
{
	RenderTargetDesc particleRenderTargetDesc(E_RENDER_TECHNIQUE_TYPE::Forward);
	particleRenderTargetDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	particleRenderTargetDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	particleRenderTargetDesc.useDepthStencil_ = true;
	pParticleRenderTarget_ = GRenderer->CreateRenderTarget(particleRenderTargetDesc);
	if (nullptr == pParticleRenderTarget_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Camera::InitDebugBuffer()
{
	RenderTargetDesc debugRenderTargeetDesc(E_RENDER_TECHNIQUE_TYPE::Forward);
	debugRenderTargeetDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	debugRenderTargeetDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	debugRenderTargeetDesc.useDepthStencil_ = true;
	pDebugRenderTarget_ = GRenderer->CreateRenderTarget(debugRenderTargeetDesc);
	if (nullptr == pDebugRenderTarget_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Camera::CameraTransformUpdate()
{
	const Transform& worldForm = GetWorldTransform();
	Vector eyePos = worldForm.GetPosition();
	Vector eyeDir = worldForm.ForwardVector();
	Vector eyeUp = worldForm.UpVector();

	MATH::MatrixLookToLH(view_, eyePos, eyeDir, eyeUp);

	const Float2& size = GetRenderSize();
	MATH::MatrixPerspectiveFovLH(projection_, fov_, (size.X / size.Y), near_, far_);
}

void Camera::CleanUp()
{
	if (nullptr != pDebugRenderTarget_)
	{
		pDebugRenderTarget_->Release();
		pDebugRenderTarget_ = nullptr;
	}
	if (nullptr != pParticleRenderTarget_)
	{
		pParticleRenderTarget_->Release();
		pParticleRenderTarget_ = nullptr;
	}
	if (nullptr != pFinalRenderTarget_)
	{
		pFinalRenderTarget_->Release();
		pFinalRenderTarget_ = nullptr;
	}

	if (nullptr != pGBufferRenderTarget_)
	{
		pGBufferRenderTarget_->Release();
		pGBufferRenderTarget_ = nullptr;
	}
}

IRenderTarget* __stdcall Camera::GetGBufferRenderTargetForEditor() const
{
	return pGBufferRenderTarget_;
}

ENGINE_API IRenderTarget* __stdcall Camera::GetParticleRenderTargetForEditor() const
{
	return pParticleRenderTarget_;
}

IRenderTarget* __stdcall Camera::GetDebugRenderTargetForEditor() const
{
	return pDebugRenderTarget_;
}

ENGINE_API IRenderTarget* __stdcall Camera::GetFinalRenderTargetForEditor() const
{
	return pFinalRenderTarget_;
}

