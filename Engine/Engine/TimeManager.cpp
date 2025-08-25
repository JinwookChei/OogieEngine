#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
//	:prevTickCount_(0)
{
	// 1�ʴ� �� CPU�� �� �� �ִ� Ŭ�� ��.
	QueryPerformanceFrequency(&frequency_);

	// ������� CPU�� �� Ŭ�� ��.
	QueryPerformanceCounter(&prevCounter_);
}

TimeManager::~TimeManager()
{
}

double TimeManager::CalcDeltaTime()
{
	LARGE_INTEGER curCounter;

	QueryPerformanceCounter(&curCounter);

	//	DeltaTime = (���� Ŭ�� ī���� - ���� Ŭ�� ī����) / CPU�� 1 �ʴ� Ŭ�� ��
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
