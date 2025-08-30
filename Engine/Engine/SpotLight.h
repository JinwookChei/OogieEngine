#pragma once

class SpotLight
	: public Light
{
public:
	SpotLight();

	~SpotLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	const DirectX::XMFLOAT3& SpotPosition() const;

	const DirectX::XMFLOAT3& SpotDirection() const;

	float SpotRange() const;

	float SpotAngle() const;

private:
	void CleanUp() override;

	DirectX::XMFLOAT3 spotPosition_;
	
	DirectX::XMFLOAT3 spotDirection_;
	
	float spotRange_;
	
	float spotAngle_;
};
