#pragma once

class SpotLight
	: public Light
{
public:
	ENGINE_API SpotLight();

	ENGINE_API ~SpotLight() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	LightRenderData GetData() override;

	Float3 Direction() const;

	float GetRange() const;

	void SetRange(float range);

private:
	void CleanUp() override;
	
	float range_;
	float smooth_;
	float innerAngle_;
	float outerAngle_;

	float attenuationConst_;
	float attenuationLinear_;
	float attenuationQuad_;


public:
	ENGINE_API int __stdcall GetLightTypeForEditor() const override;

	ENGINE_API float __stdcall GetLightRange() const override;
	ENGINE_API float __stdcall GetLightSmooth() const override;
	ENGINE_API float __stdcall GetLightInnerAngle() const override;
	ENGINE_API float __stdcall GetLightOuterAngle() const override;
	ENGINE_API void __stdcall SetLightRange(float range) override;
	ENGINE_API void __stdcall SetLightSmooth(float smooth) override;
	ENGINE_API void __stdcall SetLightInnerAngle(float innerAngle) override;
	ENGINE_API void __stdcall SetLightOuterAngle(float outerAngle) override;
};
