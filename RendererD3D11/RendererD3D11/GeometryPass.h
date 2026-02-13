#pragma once
//
//class GeometryPass final : public IUnknown
//{
//	friend class Renderer;
//
//	GeometryPass();
//
//	~GeometryPass();
//
//	bool Init();
//
//public:
//	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
//
//	ULONG __stdcall AddRef() override;
//
//	ULONG __stdcall Release() override;
//
//	void UpdatePerFrameConstant(const CameraFrameData& cameraFrameData);
//
//	void Render(const ObjectRenderData& objectData);
//
//private:
//
//	bool InitShaders();
//
//	bool InitConstantBuffer();
//
//	bool InitBlendState();
//
//	bool InitSamplerState();
//
//	bool InitDepthState();
//
//	void CleanUp();
//
//	ULONG refCount_;
//
//	Shader* pMeshShader_;
//	Shader* pSkinnedMeshShader_;
//
//	ConstantBuffer* pCBPerFrame_;
//	ConstantBuffer* pCBPerObject_;
//
//	SamplerState* pSamplerState_;
//	BlendState* pBlendState_;
//	DepthState* pDepthState_;
//};
