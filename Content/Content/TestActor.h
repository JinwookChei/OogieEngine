#pragma once


class TestActor
	: public Actor
{
public:
	TestActor();

	~TestActor() override;
	
	void BeginPlay() override;

	void Tick(double deltaTime) override;
};
