#include "stdafx.h"
#include "Level.h"
#include "World.h"

World::World()
	: curLevel_(nullptr),
	nextLevel_(nullptr)
{
}

World::~World()
{
	CleanUp();
}

void World::ChangeLevelInternal(Level* level)
{
	if (nullptr == level) {
		DEBUG_BREAK();
		return;
	}
	if (nullptr != nextLevel_) {
		delete nextLevel_;
		nextLevel_ = nullptr;
	}
	nextLevel_ = level;
}

void World::OnTick(double deltaTime)
{
	if (nullptr == curLevel_)
	{
		DEBUG_BREAK();
		return;
	}
	curLevel_->OnTick(deltaTime);
}

void World::OnRender()
{
	if (nullptr == curLevel_)
	{
		DEBUG_BREAK();
		return;
	}
	curLevel_->OnRender();
}

void World::OnBlit()
{
	curLevel_->BlitCameraToBackBuffer();
}

void World::CheckChangeLevel()
{
	if (nullptr != nextLevel_) {
		GMainCamera = nullptr;

		nextLevel_->BeginPlay();

		//GMainCamera.
		if (nullptr == GMainCamera)
		{
			DEBUG_BREAK();
			return;
		}

		ImguiSystem::GetImguiManager()->BindCamera(GMainCamera);

		//// Albedo
		//ImguiWidgetDesc widgetDesc;
		//IRenderTarget* pMainCamGBuffer = GMainCamera->GetGBufferTarget();
		//void* pAlbedoResourceView = pMainCamGBuffer->GetShaderResourceView(ERenderTextureType::Albedo);
		//widgetDesc.widgetType = EWidgetType::TextureWidget;
		//widgetDesc.text = "AlbedoTexture";
		//widgetDesc.textureResourceView = pAlbedoResourceView;
		//ImguiSystem::GetImguiManager()->CreateWidget(widgetDesc);

		//// Normal
		//void* pNormalResourceView = pMainCamGBuffer->GetShaderResourceView(ERenderTextureType::Normal);
		//widgetDesc.widgetType = EWidgetType::TextureWidget;
		//widgetDesc.text = "NormalTexture";
		//widgetDesc.textureResourceView = pNormalResourceView;
		//ImguiSystem::GetImguiManager()->CreateWidget(widgetDesc);

		//// Specular
		//void* pSpecularResourceView = pMainCamGBuffer->GetShaderResourceView(ERenderTextureType::Specular);
		//widgetDesc.widgetType = EWidgetType::TextureWidget;
		//widgetDesc.text = "SpecularTexture";
		//widgetDesc.textureResourceView = pSpecularResourceView;
		//ImguiSystem::GetImguiManager()->CreateWidget(widgetDesc);

		//// Depth
		//void* pDepthResourceView = pMainCamGBuffer->GetShaderResourceView(ERenderTextureType::Depth);
		//widgetDesc.widgetType = EWidgetType::TextureWidget;
		//widgetDesc.text = "DepthTexture";
		//widgetDesc.textureResourceView = pDepthResourceView;
		//ImguiSystem::GetImguiManager()->CreateWidget(widgetDesc);


		if (nullptr != curLevel_) {
			delete curLevel_;
			curLevel_ = nullptr;
		}

		curLevel_ = nextLevel_;
		nextLevel_ = nullptr;
	}
}

void World::CleanUp()
{
	if (nullptr != nextLevel_)
	{
		delete nextLevel_;
		nextLevel_ = nullptr;
	}

	if (nullptr != curLevel_)
	{
		delete curLevel_;
		curLevel_ = nullptr;
	}
}

