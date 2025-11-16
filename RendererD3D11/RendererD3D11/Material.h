#pragma once

class VertexShader;
class PixelShader;
class SamplerState;

class Material
	: public IMaterial
{
private:
	friend Renderer;

	Material();

	virtual ~Material();

	bool Init
	(
		VertexShader* pVertexShader, 
		PixelShader* pPixelShader, 
		//SamplerState* pSamplerState, 
		float shineness,
		Float3 specularColor
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

	IShader* __stdcall GetVertexShader() override;

	float GetShineness() const override;

	void SetShineness(float shineness) override;

	const Float3& GetSpecularColor() const override;

	void SetSpecularColor(const Float3& specularColor) override;

private:
	void CleanUp();

	ULONG refCount_;

	VertexShader* pVertexShader_;

	PixelShader* pPixelShader_;

	// SamplerState* pSamplerState_;

	float shineness_;

	Float3 specularColor_;
};
