#pragma once

struct CBPerFrame
{
	Float4x4 view;
	Float4x4 projection;

	Float4x4 inverseView;
	Float4x4 inverseProjection;

	Float2 screenOffset;
	Float2 screenScale;
	Float2 screenResolution;
	Float2 pad0;

	Float4 camPos;
};

struct CBPerObject
{
	Float4x4 world;
	Float3 materialSpecular;
	float materialShineness;
};


class ConstantBuffer final
	: public IUnknown
{
private:
	friend Renderer;

	ConstantBuffer();

	virtual ~ConstantBuffer();

	bool Init(ID3D11Buffer* pBuffer);

	bool Init(uint32_t bufferSize);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	

	void Update(void* pSrcData);

	void BindConstantBufferCS(UINT slot);
	void BindConstantBufferVS(UINT slot);
	void BindConstantBufferGS(UINT slot);
	void BindConstantBufferPS(UINT slot);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11Buffer* pBuffer_;

public:
	static void InitGlobalConstant();
	static void ShutDown();
	static ConstantBuffer* Create(uint32_t bufferSize);
	static ConstantBuffer* GConstantPerFrame;
	static ConstantBuffer* GConstantPerObject;
};

