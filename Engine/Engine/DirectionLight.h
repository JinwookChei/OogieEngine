#pragma once
enum class ELightType
{
	DirectionLight = 0,
	SpotLight,
	PointLight
};

class DirectionLight
	: public Light
{
public:
	ENGINE_API DirectionLight();

	ENGINE_API ~DirectionLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void BindLight() override;

	ENGINE_API Float3 Direction() const;

private:
	void CleanUp() override;
};