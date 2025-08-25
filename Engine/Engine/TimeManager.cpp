#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
//	:prevTickCount_(0)
{
	// 1초당 내 CPU가 돌 수 있는 클럭 수.
	QueryPerformanceFrequency(&frequency_);

	// 현재까지 CPU가 돈 클럭 수.
	QueryPerformanceCounter(&prevCounter_);
}

TimeManager::~TimeManager()
{
}

double TimeManager::CalcDeltaTime()
{
	LARGE_INTEGER curCounter;

	QueryPerformanceCounter(&curCounter);

	//	DeltaTime = (지금 클럭 카운터 - 이전 클럭 카운터) / CPU의 1 초당 클럭 수
	double deltaTime = double(curCounter.QuadPart - prevCounter_.QuadPart) / static_cast<double>(frequency_.QuadPart);

	prevCounter_ = curCounter;

	return deltaTime;



	// Ragacy
	//unsigned long long curTickCount = GetTickCount64();
	//if (prevTickCount_ == 0)
	//{
	//	prevTickCount_ = curTickCount;
	//}
	//unsigned long long deltaTime = curTickCount - prevTickCount_;
	//prevTickCount_ = curTickCount;
	//if (deltaTime < 16)
	//{
	//}
	//else if (20 <= deltaTime)
	//{
	//	deltaTime = 16;
	//}

	//return deltaTime;
}
