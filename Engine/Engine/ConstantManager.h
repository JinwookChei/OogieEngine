#pragma once

struct CBPerFrame
{
	Float4x4 view;
	Float4x4 projection;

	Float4x4 inverseView;
	Float4x4 inverseProjection;

	Float2 screenOffset;
	Float2 screenScale;
	Float2 screenResolution;
	Float2 pad0;

	Float4 camPos;
};

struct CBPerObject
{
	Float4x4 world;

	Float3 materialSpecular;
	float materialShineness;
};

struct CBPerLight
{
	Color lightDiffuse;
	Color lightSpecular;
	Color lightAmbient;

	Float3 direction_D_S; // Spot, DirectionÀÌ¶û °øÀ¯.
	float range_S_P;
	Float3 position_S_P;

	float exponent_S;
	float innerAngle_S;
	float outerAngle_S;

	float attenuationConst_S_P;
	float attenuationLinear_S_P;
	float attenuationQuad_S_P;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	float lightType;

	Float2 pad;
};


//struct CBPerMergeFrame
//{
//	Float4x4 invProjectTransform;
//	Float4x4 invViewTransform;
//
//	Float2 offset;
//	Float2 scale;
//
//	Float4 lightColor;
//	Float4 ambientColor;
//
//	Float3 spotPosition;
//	float spotAngle;
//	Float3 spotDirection;
//	float spotRange;
//};




class ConstantManager
{
public:
	ConstantManager();

	virtual ~ConstantManager();

	static ConstantManager* Instance();

	void UpdatePerFrame(CBPerFrame* pCBPerFrame);

	void UpdatePerObejct(CBPerObject* pCBPerObject);

	void UpdatePerLight(CBPerLight* pCBPerLight);

private:
	void CleanUp();

	IConstantBuffer* pPerFrameBuffer_;

	IConstantBuffer* pPerObjectBuffer_;

	IConstantBuffer* pPerLightBuffer_;
};