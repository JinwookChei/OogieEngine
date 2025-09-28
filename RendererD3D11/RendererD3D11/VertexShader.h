#pragma once


class D3D11VertexShader
	: public D3D11BaseShader
{	
public:
	D3D11VertexShader();

	~D3D11VertexShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11VertexShader* pShader_;
};