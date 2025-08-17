#pragma once

class RenderTarget;
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

	IInputLayout* __stdcall CreateLayout(IVertex* vertex, IShader* vertexShader) override;

	IVertex* __stdcall CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) override;

	IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) override;

	IShader* __stdcall CreateShader(ShaderType shaderType, const wchar_t* path) override;

	IMaterial* __stdcall CreateMaterial() override;

	ISamplerState* __stdcall  CreateSampler(bool linear, bool clamp) override;
	
	IRasterizer* __stdcall  CreateRasterizer(bool back) override;

	ID3D11Device* Device();

	ID3D11DeviceContext* DeviceContext();

	void IncrementDrawCall();

private:
	IDXGIAdapter* GetBestAdapter();

	bool CreateSwapChain(UINT width, UINT height);

	bool CreateRenderTarget();

	void Cleanup();

	bool coInit_;

	ULONG refCount_;

	uint64_t drawCallCount_;

	HWND hWnd_;

	ID3D11Device* device_;

	ID3D11DeviceContext* deviceContext_;

	IDXGISwapChain* swapChain_;

	RenderTarget* backBuffer_;
};