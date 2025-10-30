#pragma once

class PointLight
	: public Light
{
public:
	ENGINE_API PointLight();

	ENGINE_API ~PointLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void BindLight() override;

	float GetRange() const;

	void SetRange(float range);

private:
	void CleanUp() override;

	float range_;
	float attenuationConst_;
	float attenuationLinear_;
	float attenuationQuad_;
};