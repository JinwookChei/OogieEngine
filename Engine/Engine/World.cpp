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

World* World::Instance()
{
	return GWorld;
}

Level* World::GetLevel() const
{
	return curLevel_;
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

		ImGuiSystem::GetImGuiManager()->BindCamera(GMainCamera);

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

