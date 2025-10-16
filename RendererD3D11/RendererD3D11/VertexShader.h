#pragma once


class VertexShader
	: public BaseShader
{	
public:
	VertexShader();

	~VertexShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11VertexShader* pShader_;
};