#include "stdafx.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "Camera.h"

Camera::Camera()
	: fov_(60.0f),
	//size_({ 2560.f, 1440.0f }),
	near_(0.01f),
	far_(100.0f),
	//clearColor_({ 1.0f, 0.0f, 0.0f, 1.0f }),
	cameraSensitivity_(10.0f),
	cameraSpeed_(2.0f),
	pScreenVertex_(nullptr),
	pScreenMaterial_(nullptr),
	pScreenInputLayout_(nullptr),
	pScreenConstantBuffer_(nullptr),
	pRasterizer_(nullptr),
	screenOffset_({ 0.0f, 0.0f }),
	screenScale_({ 1.0f, 1.0f })
{
	MatrixIdentity(view_);
	MatrixIdentity(projection_);

	RenderTargetDesc desc;
	desc.size_ = { 2560.f, 1440.0f };
	desc.clearColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	desc.useDepthStencil_ = true;
	pRenderTarget_ = GRenderer->CreateRenderTarget(desc);

	InitScreenRect();
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

void Camera::CameraRenderBegin()
{
	CameraTransformUpdate();

	pRenderTarget_->Clear();

	pRenderTarget_->Setting();
}

void Camera::BlitToBackBuffer()
{
	BlitToBackBuffer(screenOffset_, screenScale_);
}

void Camera::BlitToBackBuffer(const Float2& offset, const Float2& scale)
{
	pRenderTarget_->BindRenderTextureForPS(0);

	pScreenVertex_->Setting();

	pScreenMaterial_->Setting();

	pScreenInputLayout_->Setting();

	ScreenRectConstant cb;
	cb.offset = offset;
	cb.scale = scale;

	pScreenConstantBuffer_->Update(&cb);
	pScreenConstantBuffer_->VSSetting(0);

	pRasterizer_->Setting();

	pScreenVertex_->Draw();

	pRenderTarget_->ClearRenderTextureForPS(0);
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
	RenderTargetDesc desc;
	if (nullptr != pRenderTarget_)
	{
		desc = pRenderTarget_->GetDesc();
		pRenderTarget_->Release();
		pRenderTarget_ = nullptr;
	}

	desc.size_ = size;
	pRenderTarget_ = GRenderer->CreateRenderTarget(desc);
}


void Camera::SetConfig(float fov, float Near, float Far)
{
	fov_ = fov;
	near_ = Near;
	far_ = Far;
}

void Camera::SetClearColor(const Color& clearColor)
{
	pRenderTarget_->SetClearColor(clearColor);
}

void Camera::SetScreenPlacement(const Float2& screenOffset, const Float2& screenScale)
{
	screenOffset_ = screenOffset;
	screenScale_ = screenScale;
}

Float2 Camera::GetRenderSize() const
{
	return pRenderTarget_->GetSize();
}


void Camera::InitScreenRect()
{
	std::vector<ScreenRectVertex> vertices;
	std::vector<WORD> indices;
	GeometryGenerator::CreateScreenRect(&vertices, &indices);
	pScreenVertex_ = GRenderer->CreateMesh
	(
		vertices.data(), (uint32_t)sizeof(ScreenRectVertex), (uint32_t)vertices.size(),
		indices.data(), (uint32_t)sizeof(WORD), (uint32_t)indices.size()
	);

	pScreenVertex_->AddInputLayout("POSITION", 0, 16, 0, false);
	pScreenVertex_->AddInputLayout("TEXCOORD", 0, 16, 0, false);

	pScreenMaterial_ = GRenderer->CreateMaterial(L"ScreenVertexShader.cso", L"ScreenPixelShader.cso", true, false);

	pScreenConstantBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(ScreenRectConstant));

	pScreenInputLayout_ = GRenderer->CreateLayout(pScreenVertex_, pScreenMaterial_->GetVertexShader());

	pRasterizer_ = GRenderer->CreateRasterizer(true, false);
	pRasterizer_->SetFillMode(EFillModeType::Solid);
}

void Camera::CameraTransformUpdate()
{
	const Transform& worldForm = GetWorldTransform();
	Vector eyePos = worldForm.GetPosition();
	Vector eyeDir = worldForm.ForwardVector();
	Vector eyeUp = worldForm.UpVector();

	MatrixLookToLH(view_, eyePos, eyeDir, eyeUp);

	float fovRad = ConvertDegToRad(fov_);

	const Float2& size = GetRenderSize();
	MatrixPerspectiveFovLH(projection_, fov_, (size.X / size.Y), near_, far_);
}

void Camera::CleanUp()
{
	if (nullptr != pRasterizer_)
	{
		pRasterizer_->Release();
		pRasterizer_ = nullptr;
	}
	if (nullptr != pScreenConstantBuffer_)
	{
		pScreenConstantBuffer_->Release();
		pScreenConstantBuffer_ = nullptr;
	}
	if (nullptr != pScreenInputLayout_)
	{
		pScreenInputLayout_->Release();
		pScreenInputLayout_ = nullptr;
	}
	if (nullptr != pScreenMaterial_)
	{
		pScreenMaterial_->Release();
		pScreenMaterial_ = nullptr;
	}
	if (nullptr != pScreenVertex_)
	{
		pScreenVertex_->Release();
		pScreenVertex_ = nullptr;
	}
	if (nullptr != pRenderTarget_)
	{
		pRenderTarget_->Release();
		pRenderTarget_ = nullptr;
	}
}

