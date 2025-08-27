#pragma once

struct SimpleVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

class VertexBuffer
	: public IVertex
{
	friend D3D11Renderer;
public:
	VertexBuffer();

	virtual ~VertexBuffer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) override;

	void __stdcall Setting() override;

	bool __stdcall Draw() override;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDesc() const;

private:
	bool Initialize(void* pVertices, UINT vertexSize, UINT vertexCount, void* pIndices = nullptr, UINT indexTypeSize = 0, UINT indexCount = 0);

	void CleanUp();

	UINT FormatSize(DXGI_FORMAT format);

	ULONG refCount_;

	UINT stride_;

	UINT offset_;

	UINT indexCount_;

	ID3D11Buffer* pVertexBuffer_;

	ID3D11Buffer* pIndexBuffer_;

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc_;
};
