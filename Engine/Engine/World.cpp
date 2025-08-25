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

void World::UpdateTick(double deltaTime)
{
	curLevel_->UpdateTick(deltaTime);
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

void World::CheckChangeLevel()
{
	if (nullptr != nextLevel_) {
		nextLevel_->BeginPlay();

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

