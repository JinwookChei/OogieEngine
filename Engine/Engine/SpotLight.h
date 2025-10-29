#pragma once

class SpotLight
	: public Light
{
public:
	ENGINE_API SpotLight();

	ENGINE_API ~SpotLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	const Float3& SpotPosition() const;

	const Float3& SpotDirection() const;

	float SpotRange() const;

	float SpotAngle() const;

private:
	void CleanUp() override;

	Float3 spotPosition_;
	
	Float3 spotDirection_;
	
	float spotRange_;
	
	float spotAngle_;
};
