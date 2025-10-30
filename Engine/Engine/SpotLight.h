#pragma once

class SpotLight
	: public Light
{
public:
	ENGINE_API SpotLight();

	ENGINE_API ~SpotLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void BindLight() override;

	Float3 Direction() const;

	float GetRange() const;

	void SetRange(float range);

	float GetAngle() const;		// Radian ¸®ÅÏ

	void SetAngle(float deg);

private:
	void CleanUp() override;
	
	float range_;
	
	float angle_;		// Rad °ª.
};
