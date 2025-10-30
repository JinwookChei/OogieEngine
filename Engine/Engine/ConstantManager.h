#pragma once

struct CBPerFrame
{
	Float4x4 view;
	Float4x4 projection;

	Float4x4 inverseView;
	Float4x4 inverseProjection;

	Float2 screenOffset;
	Float2 screenScale;

	Float4 camPos;
};

struct CBPerObject
{
	Float4x4 world;
};

struct CBPerLight
{
	Color lightDiffuse;
	Color lightSpecular;
	Color lightAmbient;

	// DirectionLight
	Float3 lightDirection; // spotDirectionÀÌ¶û °øÀ¯.

	// SpotLight
	float spotRange;
	Float3 spotPosition;
	float spotAngle;

	// PointLight
	Float3 pointPosition;
	float pointRange;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuad;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	float lightType;

	//Float4 lightColor;
	//Float4 ambientColor;

	//Float3 spotPosition;
	//float spotAngle;
	//Float3 spotDirection;
	//float spotRange;
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

	//void UpdatePerMergeFrame(CBPerMergeFrame* pCBPerMergeFrame);

	void UpdatePerObejct(CBPerObject* pCBPerObject);

	void UpdatePerLight(CBPerLight* pCBPerLight);

private:
	void CleanUp();

	IConstantBuffer* pPerFrameBuffer_;

	//IConstantBuffer* pPerMergeFrameBuffer_;

	IConstantBuffer* pPerObjectBuffer_;

	IConstantBuffer* pPerLightBuffer_;
};