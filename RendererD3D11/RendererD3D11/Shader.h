#pragma once

class InputLayout;
class Shader : public IUnknown
{
private:
	friend class Renderer;

	Shader();

	virtual ~Shader();

	bool Init(const ShaderDesc& desc);
public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static Shader* Create(const ShaderDesc& desc);

	void Bind();

	void UnBind();

private:
	UINT InputFormatSize(DXGI_FORMAT format);

	virtual void CleanUp();

private:
	ULONG refCount_;

	ID3D11InputLayout* pInputLayout_;
	ID3D11ComputeShader* pCS_;
	ID3D11VertexShader* pVS_;
	ID3D11GeometryShader* pGS_;
	ID3D11PixelShader* pPS_;
};



