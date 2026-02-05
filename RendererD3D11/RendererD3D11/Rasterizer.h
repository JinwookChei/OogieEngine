#pragma once

class Rasterizer final : public IRasterizer
{
private:
	friend class RenderResourceFactory;

	Rasterizer();
	virtual ~Rasterizer();
	bool Init(ID3D11RasterizerState* pSolidState, ID3D11RasterizerState* pWireframeState);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

	void __stdcall ChangeFillMode(E_FILLMODE_TYPE fillmode) override;

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11RasterizerState* pCurrentState_;
	ID3D11RasterizerState* pSolidState_;
	ID3D11RasterizerState* pWireframeState_;
};