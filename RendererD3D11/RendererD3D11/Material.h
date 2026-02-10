#pragma once

class Shader;
class Texture;

class Material : public IMaterial
{
private:
	friend class RenderResourceFactory;

	Material();
	virtual ~Material();

	// TODO : 이거 지워야함.
	bool Init
	(
		float shineness,
		Float3 specularColor
	);

	static IMaterial* Create(const MaterialDesc& desc);
	bool Init(const MaterialDesc& desc);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

	//void __stdcall Setting() override;

	float GetShineness() const override;

	void SetShineness(float shineness) override;

	const Float3& GetSpecularColor() const override;

	void SetSpecularColor(const Float3& specularColor) override;

	void Bind();

private:
	void CleanUp();

	ULONG refCount_;

	//IShader* pShader_;

	E_SHADER_PRESET shaderType_;
	ULONG textureCount_;
	Texture** ppTextures_;
	//Texture* pTextureAlbedo_;
	//Texture* pTextureNormal_;

	// 얘네도 필요 없음.
	float shineness_;
	Float3 specularColor_;
};
