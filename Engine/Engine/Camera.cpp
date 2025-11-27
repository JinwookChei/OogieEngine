#include "stdafx.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "Camera.h"

Camera::Camera()
	: fov_(DEFAULT_FOV),
	near_(DEFAULT_NEAR),
	far_(DEFAULT_FAR),
	cameraSensitivity_(10.0f),
	cameraSpeed_(2.0f),
	pGBufferTarget_(nullptr),
	pLightBufferTarget_(nullptr),
	pLightBufferMaterial_(nullptr),
	pDebugBufferTarget_(nullptr),
	pDebugBufferMaterial_(nullptr),
	pScreenVertex_(nullptr),
	pScreenMaterial_(nullptr),
	pScreenInputLayout_(nullptr),
	screenOffset_({ 0.0f, 0.0f }),
	screenScale_({ 1.0f, 1.0f })
{
	if (nullptr == GMainCamera)
	{
		GMainCamera = this;
	}

	MATH::MatrixIdentity(view_);

	MATH::MatrixIdentity(projection_);

	InitGBuffer();

	InitLightBuffer();

	InitDebugBuffer();

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
	//pGBufferTarget_->BindRenderTextureForPS(0);
	pLightBufferTarget_->BindRenderTextureForPS(4);
	
	pScreenVertex_->Setting();
	pScreenMaterial_->Setting();
	pScreenInputLayout_->Setting();
	pScreenVertex_->Draw();

	pLightBufferTarget_->ClearRenderTextureForPS(4);
	//pGBufferTarget_->ClearRenderTextureForPS(0);
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

IRenderTarget* Camera::GetGBufferTarget() const
{
	return pGBufferTarget_;
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
	cbPerFrame.screenResolution = pGBufferTarget_->GetSize();

	ConstantManager::Instance()->UpdatePerFrame(&cbPerFrame);
}

bool Camera::InitGBuffer()
{
	RenderTargetDesc gBufferDesc(E_RENDER_TECHNIQUE_TYPE::Deferred);
	gBufferDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
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
	RenderTargetDesc lightBufferDesc(E_RENDER_TECHNIQUE_TYPE::Forward);
	lightBufferDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	lightBufferDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	lightBufferDesc.forwardDesc_.useDepthStencil_ = false;						// 라이트 패스에서 여러 라이트를 처리하기 위해서는 Depth는 꺼야함.
	pLightBufferTarget_ = GRenderer->CreateRenderTarget(lightBufferDesc);
	if (nullptr == pLightBufferTarget_)
	{
		return false;
	}

	MaterialDesc matDesc;
	matDesc.VS = L"ScreenMergeVS.cso";
	matDesc.PS = L"DeferredLightPS.cso";
	pLightBufferMaterial_ = GRenderer->CreateMaterial(matDesc);
	if (nullptr == pLightBufferMaterial_)
	{
		return false;
	}

	return true;
}

bool Camera::InitDebugBuffer()
{
	RenderTargetDesc debugBufferDesc(E_RENDER_TECHNIQUE_TYPE::Forward);
	debugBufferDesc.size_ = { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT };
	debugBufferDesc.clearColor_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	debugBufferDesc.forwardDesc_.useDepthStencil_ = true;
	pDebugBufferTarget_ = GRenderer->CreateRenderTarget(debugBufferDesc);
	if (nullptr == pDebugBufferTarget_)
	{
		DEBUG_BREAK();
		return false;
	}

	MaterialDesc matDesc;
	matDesc.VS = L"ScreenMergeVS.cso";
	matDesc.PS = L"DeferredDebugPS.cso";
	pDebugBufferMaterial_ = GRenderer->CreateMaterial(matDesc);
	if (nullptr == pDebugBufferMaterial_)
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

	MeshDesc meshDesc;
	meshDesc.vertexFormat = E_VERTEX_FORMAT::SCREEN_RECT;
	meshDesc.vertexFormatSize = sizeof(ScreenRectVertex);
	meshDesc.vertexCount = vertices.size();
	meshDesc.vertices = vertices.data();
	meshDesc.indexFormatSize = sizeof(WORD);
	meshDesc.indexCount = indices.size();
	meshDesc.indices = indices.data();
	pScreenVertex_ = GRenderer->CreateMesh(meshDesc);

	pScreenVertex_->AddInputLayout("POSITION", 0, 16, 0, false);
	pScreenVertex_->AddInputLayout("TEXCOORD", 0, 16, 0, false);

	MaterialDesc matDesc;
	matDesc.VS = L"ScreenMergeVS.cso";
	matDesc.PS = L"ScreenMergePS.cso";
	pScreenMaterial_ = GRenderer->CreateMaterial(matDesc);

	pScreenInputLayout_ = GRenderer->CreateLayout(pScreenVertex_, pScreenMaterial_->GetVertexShader());
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
	if (nullptr != pDebugBufferMaterial_)
	{
		pDebugBufferMaterial_->Release();
		pDebugBufferMaterial_ = nullptr;
	}
	if (nullptr != pDebugBufferTarget_)
	{
		pDebugBufferTarget_->Release();
		pDebugBufferTarget_ = nullptr;
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

IRenderTarget* __stdcall Camera::GetGBufferTargetForImGui() const
{
	return pGBufferTarget_;
}

IRenderTarget* __stdcall Camera::GetDebugBufferTargetForImGui() const
{
	return pDebugBufferTarget_;
}

