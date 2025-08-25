#pragma once

class StartLevel
	: public Level
{
public:

	StartLevel();

	~StartLevel() override;

	void BeginPlay() override;

	void Tick(unsigned long long deltaTime) override;
};