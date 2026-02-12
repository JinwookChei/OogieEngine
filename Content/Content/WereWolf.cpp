#include "stdafx.h"
#include "WereWolf.h"

WereWolf::WereWolf()
{
}

WereWolf::~WereWolf()
{
	CleanUp();
}

void WereWolf::Tick(double deltaTime)
{
}

void WereWolf::BeginPlay()
{
	pRenderer_->Setting(5, 4, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void WereWolf::Render()
{
	pRenderer_->Render();
}

void WereWolf::CleanUp()
{
}
