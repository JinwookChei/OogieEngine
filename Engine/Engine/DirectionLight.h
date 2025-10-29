#pragma once


class DirectionLight
	: public Light
{
public:
	ENGINE_API DirectionLight();

	ENGINE_API ~DirectionLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	ENGINE_API Float3 Direction() const;

private:
	void CleanUp() override;
};