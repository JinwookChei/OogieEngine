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
	Light();

	~Light() override;

	//virtual void BindLight() = 0;

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