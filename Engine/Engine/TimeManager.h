#pragma once

class TimeManager final
{
public:
	TimeManager();

	virtual ~TimeManager();

	double CalcDeltaTime();

private:
    LARGE_INTEGER frequency_;    // 초당 카운트

    LARGE_INTEGER prevCounter_;  // 이전 카운트

    // Ragacy
	//unsigned long long prevTickCount_;
};
