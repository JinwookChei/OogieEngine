#pragma once

class PointLight
	: public Light
{
public:
	ENGINE_API PointLight();

	ENGINE_API ~PointLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	LightRenderData GetData() override;

	float GetRange() const;

	void SetRange(float range);

private:
	void CleanUp() override;

	float range_;
	float attenuationConst_;
	float attenuationLinear_;
	float attenuationQuad_;

public:
	ENGINE_API int __stdcall GetLightTypeForEditor() const override;

	ENGINE_API float __stdcall GetLightRange() const override;
	ENGINE_API void __stdcall SetLightRange(float range) override;
};