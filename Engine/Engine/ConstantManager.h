#pragma once

struct CBPerCameraFrame
{
	Float4x4 view;
	Float4x4 projection;
};

struct CBPerMergeFrame
{
	Float4x4 invProjectTransform;
	Float4x4 invViewTransform;

	Float2 offset;
	Float2 scale;

	Float4 lightColor;
	Float4 ambientColor;

	Float3 spotPosition;
	float spotAngle;
	Float3 spotDirection;
	float spotRange;
};

struct CBPerObject
{
	Float4x4 world;
};


class ConstantManager
{
public:
	ConstantManager();

	virtual ~ConstantManager();

	static ConstantManager* Instance();

	void UpdatePerCameraFrame(CBPerCameraFrame* pCBPerCameraFrame);

	void UpdatePerMergeFrame(CBPerMergeFrame* pCBPerMergeFrame);

	void UpdatePerObejct(CBPerObject* pCBPerObject);

private:
	void CleanUp();

	IConstantBuffer* pPerCameraFrameBuffer_;

	IConstantBuffer* pPerMergeFrameBuffer_;

	IConstantBuffer* pPerObjectBuffer_;
};