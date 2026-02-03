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
	pRenderer_->Create(5, 1, 8, 9);
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
