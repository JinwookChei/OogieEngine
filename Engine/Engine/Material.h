#pragma once

class Material
{
public:
	Material();

	virtual ~Material();

	IShader* pVertexShader_;

	IShader* pPixelShader_;

	IMaterial* pMaterial_;
//private:
};
