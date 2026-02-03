#include "stdafx.h"
#include "Wukong.h"

Wukong::Wukong()
{
}

Wukong::~Wukong()
{
	CleanUp();
}

void Wukong::Tick(double deltaTime)
{
}

void Wukong::BeginPlay()
{
	pRenderer_->Create(3, 1, 6, 7);
	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Wukong::Render()
{
	pRenderer_->Render();
}

void Wukong::CleanUp()
{
}
