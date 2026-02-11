#pragma once

struct InputDesc
{
	const char* semanticName_;
	uint32_t semanticIndex_;
	uint32_t format_;
	uint32_t inputSlot_;
	bool isInstanceData_;
};

struct ShaderDesc
{
	const wchar_t* pathCS_ = nullptr;
	const wchar_t* pathVS_ = nullptr;
	const wchar_t* pathGS_ = nullptr;
	const wchar_t* pathPS_ = nullptr;

	std::vector<InputDesc> inputDesc_;
};


class InputLayout;
class Shader : public IShader
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

public:
	static Shader* Create(const ShaderDesc& desc);
	static void InitGlobalShaders();
	static void ShutDown();
	static Shader* GShaderLight;
	static Shader* GShaderDeferredSimple;
	
};



