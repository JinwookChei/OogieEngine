#pragma once

class PortfolioLevel
	: public Level
{
public:
	PortfolioLevel();
	~PortfolioLevel() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
};