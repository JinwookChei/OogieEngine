#pragma once

struct CBPerFrame
{
	Float4x4 view;
	Float4x4 projection;

	Float4x4 inverseView;
	Float4x4 inverseProjection;

	Float2 screenOffset;
	Float2 screenScale;
};

struct CBPerObject
{
	Float4x4 world;
};

struct CBPerSpotLight
{
	Float4 lightColor;
	Float4 ambientColor;

	Float3 spotPosition;
	float spotAngle;
	Float3 spotDirection;
	float spotRange;
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

	void UpdatePerSpotLight(CBPerSpotLight* pCBPerSpotLight);

private:
	void CleanUp();

	IConstantBuffer* pPerFrameBuffer_;

	//IConstantBuffer* pPerMergeFrameBuffer_;

	IConstantBuffer* pPerObjectBuffer_;

	IConstantBuffer* pPerSpotLightBuffer_;
};