#pragma once
class TestLevel
	: public Level
{
public:
	TestLevel();

	~TestLevel();

	virtual void BeginPlay() override;

	virtual void Tick(unsigned long long deltaTime) override;
};
