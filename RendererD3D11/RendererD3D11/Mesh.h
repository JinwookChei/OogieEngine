#pragma once


class Mesh
	: public IMesh
{
private:
	friend Renderer;

	Mesh();

	virtual ~Mesh();

	bool Init(UINT stride, ID3D11Buffer* pVertexBuffer, UINT indexCount = 0, ID3D11Buffer* pIndexBuffer = nullptr);
	
public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) override;

	void __stdcall Setting() override;

	bool __stdcall Draw() override;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDesc() const;

private:
	void CleanUp();

	UINT FormatSize(DXGI_FORMAT format);

	ULONG refCount_;

	UINT stride_;

	UINT offset_;

	UINT indexCount_;

	std::vector<SimpleVertex> vertices_;

	std::vector<WORD> indices_;

	ID3D11Buffer* pVertexBuffer_;

	ID3D11Buffer* pIndexBuffer_;

	std::vector<D3D11_INPUT_ELEMENT_DESC> desc_;
};
