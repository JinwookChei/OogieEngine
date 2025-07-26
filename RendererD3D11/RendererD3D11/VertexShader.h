#pragma once

class VertexShader
	: public BaseShader
{
public:
	VertexShader();

	~VertexShader() override;

	bool CreateShader(void* shaderByteCode, size_t byteCodeLength) override;

	void SetShader() override;

	void CleanUp() override;

private:
	ID3D11VertexShader* shader_;
};