#pragma once


class RenderTarget;
class VertexBuffer;
class VertexShader;
class PixelShader;

class D3D11Renderer final
	: public IRenderer
{
public:
	D3D11Renderer();

	virtual ~D3D11Renderer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall Initialize(void* hWnd, UINT width, UINT height) override;

	void __stdcall BeginRender() override;

	void __stdcall EndRender() override;

	// TEMP
	void __stdcall Render() override;

	IVertex* __stdcall CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, void* indices = nullptr, UINT indexSize = 0) override;
	
	//IInputLayout* __stdcall CreateLayout() override;

	// TEMP
	bool __stdcall CreateTriangle() override;

	ID3D11Device* Device();

	ID3D11DeviceContext* DeviceContext();

private:
	IDXGIAdapter* GetBestAdapter();

	bool CreateSwapChain(UINT width, UINT height);

	bool CreateRenderTarget();

	void CleanUp();

	bool coInit_;

	ULONG refCount_;

	HWND hWnd_;

	ID3D11Device* device_;

	ID3D11DeviceContext* deviceContext_;

	IDXGISwapChain* swapChain_;

	RenderTarget* backBuffer_;

	VertexBuffer* mesh_;

	VertexShader* vertexShader_;

	PixelShader* pixelShader_;

	ID3D11InputLayout* inputLayout_;
};