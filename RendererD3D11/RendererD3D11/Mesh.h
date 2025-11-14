#pragma once

class Mesh
	: public IMesh
{
private:
	friend Renderer;

	Mesh();

	virtual ~Mesh();

	bool Init
	(
		E_VERTEX_FORMAT vertexFormat,
		uint32_t vertexFormatSize,
		uint32_t vertexCount,
		void* pVertices,
		ID3D11Buffer* pVertexBuffer,
		uint32_t indexFormatSize,	
		uint32_t indexCount,
		void* pIndices,
		ID3D11Buffer* pIndexBuffer
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) override;

	void __stdcall Setting() override;

	bool __stdcall Draw() override;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputDesc() const;

	void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const override;

private:
	void CleanUp();

	UINT InputFormatSize(DXGI_FORMAT format);

	ULONG refCount_;

	UINT offset_;

	E_VERTEX_FORMAT vertexFormat_;
	UINT vertexStride_;
	UINT vertexCount_;
	void* pVertices_;
	ID3D11Buffer* pVertexBuffer_;

	UINT indexStride_;
	UINT indexCount_;
	void* pIndices_;
	ID3D11Buffer* pIndexBuffer_;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc_;
};
