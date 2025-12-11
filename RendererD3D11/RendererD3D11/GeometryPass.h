#pragma once

class Shader;
class ConstantBuffer;
class SamplerState;
class BlendStateT;

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


class GeometryPass final : public IUnknown
{
	friend class Renderer;

	GeometryPass();

	~GeometryPass();

	bool Init();

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void UpdatePerFrameConstant(const CameraFrameData& cameraFrameData);

	void Render(const ObjectRenderData& objectData);

private:

	bool InitShaders();

	bool InitConstantBuffer();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();

	ULONG refCount_;

	Shader* pShader_;

	ConstantBuffer* pCBPerFrame_;
	ConstantBuffer* pCBPerObject_;

	SamplerState* pSamplerState_;
	BlendState* pBlendState_;
};
