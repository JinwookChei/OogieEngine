#pragma once

class PixelShader
	: public BaseShader
{
public:
	PixelShader();

	~PixelShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11PixelShader* pShader_;
};
