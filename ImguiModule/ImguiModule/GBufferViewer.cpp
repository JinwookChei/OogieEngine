#include "stdafx.h"
#include "ImGuiTextureWidget.h"
#include "GBufferViewer.h"


GBufferViewer::GBufferViewer()
	:pBoundCamera_(nullptr),
	pAlbedoWidget_(nullptr),
	pNormalWidget_(nullptr),
	pSpecularWidget_(nullptr),
	pDepthWidget_(nullptr),
	pDebugWidget_(nullptr)
{
}

GBufferViewer::~GBufferViewer()
{
	CleanUp();
}

GBufferViewer* GBufferViewer::Create(IImGuiBindCamera* pCamera)
{
	GBufferViewer* pNewGBufferViewer = new GBufferViewer;

	pNewGBufferViewer->BindCamera(pCamera);

	return pNewGBufferViewer;
}


void GBufferViewer::Render()
{
	if (false == IsActive())
	{
		return;
	}

	pAlbedoWidget_->Render();
	pNormalWidget_->Render();
	pSpecularWidget_->Render();
	pDepthWidget_->Render();
	pDebugWidget_->Render();
}

void GBufferViewer::BindCamera(IImGuiBindCamera* pCamera)
{
	if (nullptr == pCamera)
	{
		Assert("Camera is NULL");
		return;
	}

	pBoundCamera_ = pCamera;
	IRenderTarget* pGBufferTarget = pBoundCamera_->GetGBufferTargetForImGui();
	IRenderTarget* pDebugTarget = pBoundCamera_->GetDebugBufferTargetForImGui();
	
	ImTextureID* pAlbedoTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo));
	pAlbedoWidget_ = new ImGuiTextureWidget(pAlbedoTexture, "Albedo");
	ImTextureID* pNormalTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Normal));
	pNormalWidget_ = new ImGuiTextureWidget(pNormalTexture, "Normal");
	ImTextureID* pSpecularTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Specular));
	pSpecularWidget_ = new ImGuiTextureWidget(pSpecularTexture, "Specular");
	ImTextureID* pDepthTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Depth));
	pDepthWidget_ = new ImGuiTextureWidget(pDepthTexture, "Depth");

	ImTextureID* pDebugTexture = static_cast<ImTextureID*>(pDebugTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo));
	pDebugWidget_ = new ImGuiTextureWidget(pDebugTexture, "Debug");
}

void GBufferViewer::CleanUp()
{
	if (nullptr != pDebugWidget_)
	{
		pDebugWidget_->Release();
		pDebugWidget_ = nullptr;
	}
	if (nullptr != pAlbedoWidget_)
	{
		pAlbedoWidget_->Release();
		pAlbedoWidget_ = nullptr;
	}
	if (nullptr != pNormalWidget_)
	{
		pNormalWidget_->Release();
		pNormalWidget_ = nullptr;
	}
	if (nullptr != pSpecularWidget_)
	{
		pSpecularWidget_->Release();
		pSpecularWidget_ = nullptr;
	}
	if (nullptr != pDepthWidget_)
	{
		pDepthWidget_->Release();
		pDepthWidget_ = nullptr;
	}
	if (nullptr != pBoundCamera_)
	{
		pBoundCamera_ = nullptr;
	}
}