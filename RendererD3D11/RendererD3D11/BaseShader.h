#pragma once

class InputLayout;
class BaseShader : public IShader
{
private:
	friend class Renderer;

	BaseShader();

	virtual ~BaseShader();

	bool Init(const ShaderDesc& desc);

	static BaseShader* Create(const ShaderDesc& desc);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Setting() override;

	//void UnBind() override;

private:
	UINT InputFormatSize(DXGI_FORMAT format);

	virtual void CleanUp();

private:
	ULONG refCount_;

	E_SHADER_TYPE shaderType_;

	ID3D11InputLayout* pInputLayout_;
	ID3D11ComputeShader* pCS_;
	ID3D11VertexShader* pVS_;
	ID3D11GeometryShader* pGS_;
	ID3D11PixelShader* pPS_;
};



