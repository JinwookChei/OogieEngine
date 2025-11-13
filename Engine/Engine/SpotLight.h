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

//	float GetAngle() const;		// Radian ∏Æ≈œ

	//void SetAngle(float deg);

private:
	void CleanUp() override;
	
	float range_;
	
	float exponent_;
	float innerAngle_;
	float outerAngle_;

	float attenuationConst_;
	float attenuationLinear_;
	float attenuationQuad_;

	RenderComponent* pRenderer_;
};
