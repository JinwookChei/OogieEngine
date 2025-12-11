#include "stdafx.h"
#include "ImGuiTextureWidget.h"
#include "GBufferViewer.h"


GBufferViewer::GBufferViewer()
	:pBoundCamera_(nullptr)
	, pAlbedoWidget_(nullptr)
	, pNormalWidget_(nullptr)
	, pSpecularWidget_(nullptr)
	//, pDepthWidget_(nullptr)
	, pParticleWidget_(nullptr)
	, pDebugWidget_(nullptr)
{
}

GBufferViewer::~GBufferViewer()
{
	CleanUp();
}

GBufferViewer* GBufferViewer::Create()
{
	GBufferViewer* pNewGBufferViewer = new GBufferViewer;
	pNewGBufferViewer->Init();

	return pNewGBufferViewer;
}

bool GBufferViewer::Init()
{
	pAlbedoWidget_ = new ImGuiTextureWidget("Albedo");
	pNormalWidget_ = new ImGuiTextureWidget("Normal");
	pSpecularWidget_ = new ImGuiTextureWidget("Specular");
	//pDepthWidget_ = new ImGuiTextureWidget("Depth");
	pParticleWidget_ = new ImGuiTextureWidget("Particle");
	pDebugWidget_ = new ImGuiTextureWidget("Debug");

	return true;
}

void GBufferViewer::Render()
{
	if (!IsActive())
	{
		return;
	}

	pAlbedoWidget_->Render();
	pNormalWidget_->Render();
	pSpecularWidget_->Render();
	//pDepthWidget_->Render();
	pParticleWidget_->Render();
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
	IRenderTarget* pGBufferTarget = pBoundCamera_->GetGBufferRenderTargetForImGui();
	IRenderTarget* pParticleTarget = pBoundCamera_->GetParticleRenderTargetForImGui();
	IRenderTarget* pDebugTarget = pBoundCamera_->GetDebugRenderTargetForImGui();

	ImTextureID* pAlbedoTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo));
	pAlbedoWidget_->BindTexture(pAlbedoTexture);
	ImTextureID* pNormalTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Normal));
	pNormalWidget_->BindTexture(pNormalTexture);
	ImTextureID* pSpecularTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Specular));
	pSpecularWidget_->BindTexture(pSpecularTexture);
	/*ImTextureID* pDepthTexture = static_cast<ImTextureID*>(pGBufferTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Depth));
	pDepthWidget_->BindTexture(pDepthTexture);*/
	ImTextureID* pParticleTexture = static_cast<ImTextureID*>(pParticleTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo));
	pParticleWidget_->BindTexture(pParticleTexture);
	ImTextureID* pDebugTexture = static_cast<ImTextureID*>(pDebugTarget->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo));
	pDebugWidget_->BindTexture(pDebugTexture);
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
	//if (nullptr != pDepthWidget_)
	//{
	//	pDepthWidget_->Release();
	//	pDepthWidget_ = nullptr;
	//}
	if (nullptr != pParticleWidget_)
	{
		pParticleWidget_->Release();
		pParticleWidget_ = nullptr;
	}
	if (nullptr != pBoundCamera_)
	{
		pBoundCamera_ = nullptr;
	}
}