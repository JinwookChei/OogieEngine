#include "stdafx.h"
#include "TestActor.h"

TestActor::TestActor()
	:pRenderer_(new RenderComponent(this))
{
}

TestActor::~TestActor()
{
	CleanUp();
}

void TestActor::BeginPlay()
{
	pRenderer_->Create(MESH_TYPE::CUBE);
	pTransform_->SetScale({1.0f, 1.0f, 1.0f, 0.0f});
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f});
	pTransform_->SetPosition({ 0.0f, 0.0f, 1.0f, 1.0f });
}

void TestActor::Tick(double deltaTime)
{
}

void TestActor::Render()
{
	pRenderer_->Render();
}

void TestActor::CleanUp()
{
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
