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

private:
	bool Initialize(void* vertices, UINT vertexSize, void* indices = nullptr, UINT indexSize = 0);

	std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutDesc() const;

	bool AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData) override;

	UINT FormatSize(DXGI_FORMAT format);

	void CleanUp();

private:
	ULONG refCount_;

	ID3D11Buffer* vertexBuffer_;

	ID3D11Buffer* indexBuffer_;

	// Layout
	UINT offset_;

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc_;
};
