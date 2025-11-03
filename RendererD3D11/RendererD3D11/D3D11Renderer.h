#pragma once

class RenderTarget;
class BlendState;

class Renderer final
	: public IRenderer
{
public:
	Renderer();

	virtual ~Renderer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) override;

	void __stdcall RenderBegin() override;

	void __stdcall RenderEnd() override;

	uint64_t __stdcall DrawCallCount() override;

	// ------------------------- 인터페이스 노출된 Create -----------------------------
	IInputLayout* __stdcall CreateLayout(IMesh* pMesh, IShader* pVertexShader) override;

	IMesh* __stdcall CreateMesh(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) override;

	//IMaterial* __stdcall CreateMaterial(const wchar_t* VS, const wchar_t* PS, bool samplerLinear, bool samplerClamp) override;

	IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) override;

	IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) override;

	IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) override;

	//IRenderTarget* __stdcall CreateRenderTarget(const Float2& size, const Color& clearColor, bool useDepthStencil = true) override;

	IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) override;

	IRenderTarget* CreateForwardRenderTarget(const RenderTargetDesc& desc);

	IRenderTarget* CreateDeferredRenderTarget(const RenderTargetDesc& desc);

	IBlendState* CreateBlendState(uint32_t srcBlend, uint32_t destBlend, uint32_t srcBlendAlpha, uint32_t destBlendAlpha, float blendFactor[4] = nullptr) override;
	
	// ------------------------- 인터페이스 노출안된 Create -----------------------------
	IShader* CreateShader(EShaderType shaderType, const wchar_t* pPath);

	ITexture* CreateTexture(const Float2& size, DXGI_FORMAT format, uint32_t flag);

	ITexture* CreateTexture(const D3D11_TEXTURE2D_DESC& desc);

	ISamplerState*  CreateSamplerState(bool linear, bool clamp);
	

	ID3D11Device* Device();

	ID3D11DeviceContext* DeviceContext();

	void IncrementDrawCall();

private:
	IDXGIAdapter* GetBestAdapter();

	bool CreateSwapChain(UINT width, UINT height);

	bool CreateBackBuffer(UINT width, UINT height, const Color& clearColor);

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