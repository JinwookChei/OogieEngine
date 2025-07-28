#pragma once

class PixelShader
	: public BaseShader
{
	friend D3D11Renderer;
public:
	PixelShader();

	~PixelShader() override;

	bool CreateShader(void* shaderByteCode, size_t byteCodeLength) override;

	void SetShader() override;

	void CleanUp() override;

private:
	ID3D11PixelShader* shader_;
};
