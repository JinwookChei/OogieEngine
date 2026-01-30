#pragma once

class TimeManager final
{
private:
	friend class Engine;
	TimeManager();
	~TimeManager();

	static bool Init();
	static TimeManager* GTimeManager;

public:
	static double CalcDeltaTime();
	static double GetDeltaTime();
	static void CleanUp();

private:
	double deltaTime_;
	LARGE_INTEGER frequency_;    // 초당 카운트
	LARGE_INTEGER prevCounter_;  // 이전 카운트
};
