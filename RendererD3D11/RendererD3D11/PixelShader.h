#pragma once

class D3D11PixelShader
	: public D3D11BaseShader
{
public:
	D3D11PixelShader();

	~D3D11PixelShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11PixelShader* pShader_;
};
