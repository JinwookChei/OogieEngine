#include "stdafx.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "Camera.h"

Camera::Camera()
	: fov_(60.0f),
	near_(0.01f),
	far_(100.0f),
	cameraSensitivity_(10.0f),
	cameraSpeed_(2.0f),
	pLightBufferTarget_(nullptr),
	pScreenVertex_(nullptr),
	pScreenMaterial_(nullptr),
	pScreenInputLayout_(nullptr),
	pScreenRasterizer_(nullptr),
	screenOffset_({ 0.0f, 0.0f }),
	screenScale_({ 1.0f, 1.0f })
{
	MATH::MatrixIdentity(view_);

	MATH::MatrixIdentity(projection_);

	InitGBuffer();

	InitLightBuffer();

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

void Camera::GeometryPassBegin()
{
	pGBufferTarget_->Clear();
	pGBufferTarget_->Setting();
}

void Camera::GeometryPassEnd()
{
	pGBufferTarget_->EndRenderPass();
}

void Camera::LightPassBegin()
{
	pLightBufferTarget_->Clear();
	pLightBufferTarget_->Setting();

	pGBufferTarget_->BindRenderTextureForPS(0);

	pScreenVertex_->Setting();
	pScreenInputLayout_->Setting();
	pScreenRasterizer_->Setting();
	pLightBufferMaterial_->Setting();
}

void Camera::RenderLight()
{
	pScreenVertex_->Draw();
}

void Camera::LightPassEnd()
{
	pGBufferTarget_->ClearRenderTextureForPS(0);

	pLightBufferTarget_->EndRenderPass();
}

void Camera::BlitToBackBuffer()
{
	BlitToBackBuffer(screenOffset_, screenScale_);
}

void Camera::BlitToBackBuffer(const Float2& offset, const Float2& scale)
{
	pLightBufferTarget_->BindRenderTextureForPS(4);
	//pGBufferTarget_->BindRenderTextureForPS(0);
	
	pScreenVertex_->Setting();
	pScreenMaterial_->Setting();
	pScreenInputLayout_->Setting();
	pScreenRasterizer_->Setting();
	pScreenVertex_->Draw();

	//pGBufferTarget_->ClearRenderTextureForPS(0);
	pLightBufferTarget_->ClearRenderTextureForPS(4);
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
	if (nullptr == pGBufferTarget_)
	{
		Assert("pGBufferTarget_ == NULL!");
		return;
	}

	RenderTargetDesc desc = pGBufferTarget_->GetDesc();
	pGBufferTarget_->Release();
	pGBufferTarget_ = nullptr;

	desc.size_ = size;
	pGBufferTarget_ = GRenderer->CreateRenderTarget(desc);
}


void Camera::SetConfig(float fov, float Near, float Far)
{
	fov_ = fov;
	near_ = Near;
	far_ = Far;
}

void Camera::SetClearColor(const Color& clearColor)
{
	pGBufferTarget_->SetClearColor(clearColor);
}

void Camera::SetScreenPlacement(const Float2& screenOffset, const Float2& screenScale)
{
	screenOffset_ = screenOffset;
	screenScale_ = screenScale;
}

Float2 Camera::GetRenderSize() const
{
	return pGBufferTarget_->GetSize();
}

void Camera::UpdatePerFrameConstant()
{
	CameraTransformUpdate();

	Float4x4 viewTrans;
	MATH::MatrixTranspose(viewTrans, view_);
	Float4x4 projectionTrans;
	MATH::MatrixTranspose(projectionTrans, projection_);
	Float4x4 invViewTrans;
	MATH::MatrixInverse(invViewTrans, view_);
	MATH::MatrixTranspose(invViewTrans, invViewTrans);
	Float4x4 invProjectionTrans;
	MATH::MatrixInverse(invProjectionTrans, projection_);
	MATH::MatrixTranspose(invProjectionTrans, invProjectionTrans);

	CBPerFrame cbPerFrame;
	cbPerFrame.camPos = pTransform_->GetPosition();
	cbPerFrame.view = viewTrans;
	cbPerFrame.projection = projectionTrans;
	cbPerFrame.inverseView = invViewTrans;
	cbPerFrame.inverseProjection = invProjectionTrans;
	cbPerFrame.screenOffset = screenOffset_;
	cbPerFrame.screenScale = screenScale_;

	ConstantManager::Instance()->UpdatePerFrame(&cbPerFrame);
}

bool Camera::InitGBuffer()
{
	RenderTargetDesc gBufferDesc(ERenderTechniqueType::Deferred);
	gBufferDesc.size_ = { 2560.f, 1440.0f };
	gBufferDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	pGBufferTarget_ = GRenderer->CreateRenderTarget(gBufferDesc);

	if (nullptr == pGBufferTarget_)
	{
		return false;
	}

	return true;
}

bool Camera::InitLightBuffer()
{
	RenderTargetDesc lightBufferDesc(ERenderTechniqueType::Forward);
	lightBufferDesc.size_ = { 2560.f, 1440.0f };
	lightBufferDesc.clearColor_ = { 1.0f, 1.0f, 0.0f, 0.0f };
	pLightBufferTarget_ = GRenderer->CreateRenderTarget(lightBufferDesc);
	if (nullptr == pLightBufferTarget_)
	{
		return false;
	}

	pLightBufferMaterial_ = GRenderer->CreateMaterial(L"ScreenMergeVS.cso", L"DeferredLightPS.cso", true, false);
	if (nullptr == pLightBufferMaterial_)
	{
		return false;
	}

	return true;
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

	pScreenMaterial_ = GRenderer->CreateMaterial(L"ScreenMergeVS.cso", L"ScreenMergePS.cso", true, false);

	pScreenInputLayout_ = GRenderer->CreateLayout(pScreenVertex_, pScreenMaterial_->GetVertexShader());

	pScreenRasterizer_ = GRenderer->CreateRasterizer(true, false);
	pScreenRasterizer_->SetFillMode(EFillModeType::Solid);
}

void Camera::CameraTransformUpdate()
{
	const Transform& worldForm = GetWorldTransform();
	Vector eyePos = worldForm.GetPosition();
	Vector eyeDir = worldForm.ForwardVector();
	Vector eyeUp = worldForm.UpVector();

	MATH::MatrixLookToLH(view_, eyePos, eyeDir, eyeUp);

	float fovRad = MATH::ConvertDegToRad(fov_);

	const Float2& size = GetRenderSize();
	MATH::MatrixPerspectiveFovLH(projection_, fov_, (size.X / size.Y), near_, far_);
}

void Camera::CleanUp()
{
	if (nullptr != pScreenRasterizer_)
	{
		pScreenRasterizer_->Release();
		pScreenRasterizer_ = nullptr;
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

	if (nullptr != pLightBufferMaterial_)
	{
		pLightBufferMaterial_->Release();
		pLightBufferMaterial_ = nullptr;
	}

	if (nullptr != pLightBufferTarget_)
	{
		pLightBufferTarget_->Release();
		pLightBufferTarget_ = nullptr;
	}

	if (nullptr != pGBufferTarget_)
	{
		pGBufferTarget_->Release();
		pGBufferTarget_ = nullptr;
	}
}

