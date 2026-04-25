#pragma once
enum class E_LIGHT_TYPE
{
	DIRECTION_LIGHT = 0,
	SPOT_LIGHT,
	POINT_LIGHT	
};

class StaticMeshComponent;
class Light
	: public Actor
{
public:
	static IPSO* GLightPSO;
	static IPSO* GAmbientPSO;

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

	float intensity_;

	Color diffuseColor_;

	Color specularColor_;

	Color ambientColor_;

public:	
	ENGINE_API Color __stdcall GetDiffuseColorForEditor() const override;
	ENGINE_API Color __stdcall GetSpecularColorForEditor() const override;
	ENGINE_API Color __stdcall GetAmbientColorForEditor() const override;
	ENGINE_API void __stdcall SetDiffuseColorForEditor(const Color& color) override;
	ENGINE_API void __stdcall SetSpecularColorForEditor(const Color& color) override;
	ENGINE_API void __stdcall SetAmbientColorForEditor(const Color& color) override;

	ENGINE_API float __stdcall GetLightIntensity() const override;
	ENGINE_API void __stdcall SetLightIntensity(float intensity) override;
};