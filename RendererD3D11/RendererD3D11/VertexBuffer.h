#pragma once

class VertexBuffer
	: public IVertex
{
public:
	VertexBuffer();

	virtual ~VertexBuffer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

private:
	bool Initialize(void* vertices, UINT vertexSize, void* indices = nullptr, UINT indexSize = 0);

	void CleanUp();

private:
	ULONG refCount_;

	ID3D11Buffer* vertexBuffer_;

	ID3D11Buffer* indexBuffer_;

};
