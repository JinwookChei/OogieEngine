#pragma once

class DirectionLight
	: public Light
{
public:
	ENGINE_API DirectionLight();

	ENGINE_API ~DirectionLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void BindLight() override;

	LightRenderData GetData() override;

	Float3 Direction() const;

private:
	void CleanUp() override;
};