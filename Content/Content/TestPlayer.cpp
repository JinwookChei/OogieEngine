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
	/*pRenderer_->Create(4, 1, 4, 5);
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 90.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 8.0f, 1.0f });*/


	pRenderer_->Create(4, 1, 4, 5);
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
