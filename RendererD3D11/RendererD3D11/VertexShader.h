#pragma once


class VertexShader
	: public BaseShader
{	
protected:
	friend class Renderer;

	VertexShader();

	~VertexShader() override;

protected:
	void SetShader() override;

	bool OnCreateShader(ID3DBlob* pBlob) override;

	void CleanUp() override;

private:
	ID3D11VertexShader* pShader_;
};