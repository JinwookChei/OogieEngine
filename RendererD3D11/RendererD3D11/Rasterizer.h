#pragma once

class D3D11Rasterizer final
	: public IRasterizer
{
public:
	D3D11Rasterizer();

	virtual ~D3D11Rasterizer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

	void __stdcall SetFillMode(FillModeType fillmode) override;

	bool CreateRasterizer(bool frontCounterClockwise, bool backFace);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11RasterizerState* pCurrentState_;

	ID3D11RasterizerState* pSolidState_;

	ID3D11RasterizerState* pWireframeState_;
};