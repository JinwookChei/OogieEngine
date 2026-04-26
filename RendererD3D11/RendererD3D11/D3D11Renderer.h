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
	void __stdcall UpdateComputeParticleFrame(const ComputeParticleData& computeParticleData) override;
	void __stdcall UpdateRenderParticleFrame(const RenderParticleData& renderParticleData) override;
	void __stdcall UpdateAnimationFrame(const AnimConstantBuffer& animFrameData) override;
	void __stdcall UpdateMaterialFrame(const Float3& specularColor, float shineness);

	void __stdcall RenderBegin() override;
	void __stdcall Render(IPSO* pipelineStateObject) override;
	void __stdcall RenderParticle_Test(IPSO* pipelineStateObject) override;
	void __stdcall Compute(IPSO* pipelineStateObject, UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ) override;

	void __stdcall UnBindSRVs(bool bVS, bool bPS) override;
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
	void CleanUp();

	bool coInit_;
	ULONG refCount_;
	uint64_t drawCallCount_;
	HWND hWnd_;
	ID3D11Device* pDevice_;
	ID3D11DeviceContext* pDeviceContext_;
	IDXGISwapChain* pSwapChain_;
	RenderTarget* pBackBuffer_;
};