#pragma once

class PixelShader
	: public BaseShader
{
public:
	PixelShader();

	~PixelShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* blob) override;

	void OnCleanup() override;

private:
	ID3D11PixelShader* shader_;
};
