#pragma once

class TimeManager final
{
public:
	TimeManager();

	virtual ~TimeManager();

	double CalcDeltaTime();

private:
    LARGE_INTEGER frequency_;    // �ʴ� ī��Ʈ

    LARGE_INTEGER prevCounter_;  // ���� ī��Ʈ

    // Ragacy
	//unsigned long long prevTickCount_;
};
