#include "stdafx.h"
#include "ImGuiTextureWidget.h"
#include "GBufferViewer.h"


GBufferViewer::GBufferViewer()
	:pBoundCamera_(nullptr),
	pAlbedoWidget_(nullptr),
	pNormalWidget_(nullptr),
	pSpecularWidget_(nullptr),
	pDepthWidget_(nullptr)
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
	ImTextureID* pAlbedoTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(ERenderTextureType::Albedo));
	pAlbedoWidget_ = new ImGuiTextureWidget(pAlbedoTexture, "Albedo");
	ImTextureID* pNormalTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(ERenderTextureType::Normal));
	pNormalWidget_ = new ImGuiTextureWidget(pNormalTexture, "Normal");
	ImTextureID* pSpecularTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(ERenderTextureType::Specular));
	pSpecularWidget_ = new ImGuiTextureWidget(pSpecularTexture, "Specular");
	ImTextureID* pDepthTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(ERenderTextureType::Depth));
	pDepthWidget_ = new ImGuiTextureWidget(pDepthTexture, "Depth");
}

void GBufferViewer::CleanUp()
{
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