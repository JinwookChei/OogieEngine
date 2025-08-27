#pragma once


class Shader
{
public:
	Shader();

	Shader(IShader* pShader);

	virtual ~Shader();

	void SetShader(IShader* pShader);

private:
	void CleanUp();

	IShader* pShaderImpl_;
};