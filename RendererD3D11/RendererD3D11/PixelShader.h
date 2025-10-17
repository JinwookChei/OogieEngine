#pragma once

class PixelShader
	: public BaseShader
{
protected:
	friend class Renderer;

	PixelShader();

	~PixelShader() override;

protected:
	void SetShader() override;

	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11PixelShader* pShader_;
};
