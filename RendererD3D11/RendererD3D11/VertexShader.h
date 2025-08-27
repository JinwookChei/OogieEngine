#pragma once


class VertexShader
	: public BaseShader
{	
public:
	VertexShader();

	~VertexShader() override;

	void SetShader() override;

protected:
	bool OnCreateShader(ID3DBlob* blob) override;

	void CleanUp() override;

private:
	ID3D11VertexShader* shader_;
};