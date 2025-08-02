#pragma once


class VertexShader
	: public BaseShader
{	
	friend D3D11Renderer;
public:
	VertexShader();

	~VertexShader() override;

	bool CreateShader(ID3DBlob* pBlob) override;

	void SetShader() override;

	void CleanUp() override;

private:
	ID3D11VertexShader* shader_;

};