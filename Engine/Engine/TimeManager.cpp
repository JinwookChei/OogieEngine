#include "stdafx.h"
#include "TimeManager.h"

TimeManager* TimeManager::GTimeManager = nullptr;

TimeManager::TimeManager()
	: deltaTime_(0.0)
{
}

TimeManager::~TimeManager()
{
}

bool TimeManager::Init()
{
	if (nullptr != GTimeManager)
	{
		DEBUG_BREAK();
		return false;
	}

	GTimeManager = new TimeManager;
	return true;
}

void TimeManager::StartTimer()
{
	// 1초당 내 CPU가 돌 수 있는 클럭 수.
	QueryPerformanceFrequency(&GTimeManager->frequency_);

	// 현재까지 CPU가 돈 클럭 수.
	QueryPerformanceCounter(&GTimeManager->prevCounter_);
}

double TimeManager::CalcDeltaTime()
{
	LARGE_INTEGER curCounter;
	QueryPerformanceCounter(&curCounter);

	//	DeltaTime = (지금 클럭 카운터 - 이전 클럭 카운터) / CPU의 1 초당 클럭 수
	double deltaTime  
		= static_cast<double>(curCounter.QuadPart - GTimeManager->prevCounter_.QuadPart) / static_cast<double>(GTimeManager->frequency_.QuadPart);
	GTimeManager->prevCounter_ = curCounter;
	GTimeManager->deltaTime_ = deltaTime;

	return deltaTime;
}

double TimeManager::GetDeltaTime()
{
	return GTimeManager->deltaTime_;
}

void TimeManager::CleanUp()
{
	if (nullptr != GTimeManager)
	{
		delete GTimeManager;
		GTimeManager = nullptr;
	}
}
