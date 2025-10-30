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

	float SpotRange() const;

	void SetRange(float range);

	float SpotAngle() const;

	void SetAngle(float deg);

private:
	void CleanUp() override;
	
	float spotRange_;
	
	float spotAngle_;
};
