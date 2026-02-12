#include "stdafx.h"
#include "TestPlayer.h"

TestPlayer::TestPlayer()
{
}

TestPlayer::~TestPlayer()
{
	CleanUp();
}

void TestPlayer::Tick(double deltaTime)
{
}

void TestPlayer::BeginPlay()
{
	pRenderer_->Setting(4, 2, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });


}

void TestPlayer::Render()
{
	pRenderer_->Render();
}

void TestPlayer::CleanUp()
{
}
