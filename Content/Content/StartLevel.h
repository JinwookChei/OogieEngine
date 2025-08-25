#pragma once

class StartLevel
	: public Level
{
public:

	StartLevel();

	~StartLevel() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;
};