#pragma once

class RenderTarget;
class BlendState;
class DepthState;
class GeometryPass;
class LightPass;
class ParticlePass;
class DebugPass;
class MergePass;
class FinalPass;

class Renderer final : public IRenderer
{
public:
	Renderer();

	virtual ~Renderer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) override;

	void* __stdcall GetDeviceHandle() override;

	void* __stdcall GetDeviceContextHandle() override;

	void __stdcall UpdateCameraFrame(const CameraFrameData& cameraFrameData) override;
	void __stdcall UpdateObjectFrame(const ObjectFrameData& objectFrameData) override;
	void __stdcall UpdateLightFrame(const LightRenderData& lightFrameData) override;

	void __stdcall RenderTest(IPSO* pipelineStateObject) override;

	void __stdcall UnBindSRVs(bool bVS, bool bPS) override;

	void __stdcall UpdateParticles(IParticle* pParticle, double deltaTime) override;
	void __stdcall RenderParticles(IParticle* pParticle, const Float4x4 worldTransform, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp) override;

	void __stdcall DrawDebugLine(const Float3& start, const Float3& end, const Float4& color) override;
	void __stdcall DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color) override;
	void __stdcall RenderDebug() override;

	void __stdcall RenderMerge(IRenderTarget* pSrcTarget) override;

	void __stdcall RenderBegin() override;
	void __stdcall RenderFinal(IRenderTarget* pSrcTarget) override;
	void __stdcall RenderEnd() override;

	uint64_t __stdcall DrawCallCount() override;

	void Draw(UINT count, bool useIndex);
	
	void IncrementDrawCall();

	ID3D11Device* Device();

	ID3D11DeviceContext* DeviceContext();
private:

	IDXGIAdapter* GetBestAdapter();

	bool InitSwapChain(UINT width, UINT height);

	bool InitBackBuffer(UINT width, UINT height, const Color& clearColor);

	bool InitParticlePass();

	bool InitDebugPass();

	bool InitMergePass();

	bool InitFinalPass();

	void CleanUp();

	bool coInit_;

	ULONG refCount_;

	uint64_t drawCallCount_;

	HWND hWnd_;

	ID3D11Device* pDevice_;

	ID3D11DeviceContext* pDeviceContext_;

	IDXGISwapChain* pSwapChain_;

	RenderTarget* pBackBuffer_;

	ParticlePass* pParticlePass_;

	DebugPass* pDebugPass_;

	MergePass* pMergePass_;

	FinalPass* pFinalPass_;
};