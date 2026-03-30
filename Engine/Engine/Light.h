#pragma once
enum class ELightType
{
	DirectionLight = 0,
	SpotLight,
	PointLight
};

class StaticMeshComponent;
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

	void RenderLight();

	virtual LightRenderData GetData() = 0;

	const Color& DiffuseColor() const;

	const Color& SpecularColor() const;
	
	const Color& AmbientColor() const;

private:
	void CleanUp() override;

protected:
	StaticMeshComponent* pStaticMesh_;

	Color diffuseColor_;

	Color specularColor_;

	Color ambientColor_;
};