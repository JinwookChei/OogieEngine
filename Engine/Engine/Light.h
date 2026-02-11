#pragma once
enum class ELightType
{
	DirectionLight = 0,
	SpotLight,
	PointLight
};

class Light
	: public Actor
{
public:
	static IPSO* GLightPSO;

public:
	Light();

	~Light() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void Render() override;

	virtual LightRenderData GetData() = 0;

	const Color& DiffuseColor() const;

	const Color& SpecularColor() const;
	
	const Color& AmbientColor() const;

private:
	void CleanUp() override;

	Color diffuseColor_;

	Color specularColor_;

	Color ambientColor_;
};