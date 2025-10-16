#pragma once

class D3D11RenderTarget;
class D3D11Renderer final
	: public IRenderer
{
public:
	D3D11Renderer();

	virtual ~D3D11Renderer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) override;

	void __stdcall RenderBegin() override;

	void __stdcall RenderEnd() override;

	uint64_t __stdcall DrawCallCount() override;

	IInputLayout* __stdcall CreateLayout(IVertex* pVertex, IShader* pVertexShader) override;

	IVertex* __stdcall CreateVertex(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) override;

	IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) override;

	IShader* __stdcall CreateShader(EShaderType shaderType, const wchar_t* pPath) override;

	ISamplerState* __stdcall  CreateSampler(bool linear, bool clamp) override;
	
	IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) override;

	IRenderTarget* __stdcall CreateRenderTarget(const Float2& size, const Color& clearColor, bool useDepthStencil = true) override;

	ID3D11Device* Device();

	ID3D11DeviceContext* DeviceContext();

	void IncrementDrawCall();

private:
	IDXGIAdapter* GetBestAdapter();

	bool CreateSwapChain(UINT width, UINT height);

	bool CreateBackBuffer();

	void CleanUp();

	bool coInit_;

	ULONG refCount_;

	uint64_t drawCallCount_;

	HWND hWnd_;

	ID3D11Device* pDevice_;

	ID3D11DeviceContext* pDeviceContext_;

	IDXGISwapChain* pSwapChain_;

	D3D11RenderTarget* pBackBuffer_;
};