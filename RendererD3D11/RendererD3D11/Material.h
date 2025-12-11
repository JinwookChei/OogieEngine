#pragma once

class Material
	: public IMaterial
{
private:
	friend Renderer;

	Material();

	virtual ~Material();

	bool Init
	(
		float shineness,
		Float3 specularColor
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	//void __stdcall Setting() override;

	float GetShineness() const override;

	void SetShineness(float shineness) override;

	const Float3& GetSpecularColor() const override;

	void SetSpecularColor(const Float3& specularColor) override;

private:
	void CleanUp();

	ULONG refCount_;

	float shineness_;

	Float3 specularColor_;
};
