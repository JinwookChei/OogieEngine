#pragma once


class Mesh;
class Material;
class Shader;
class InputLayout;
class ShaderConstants;
class Rasterizer;

class RenderComponent
{
public:
	RenderComponent();

	virtual ~RenderComponent();

	void Render();	

	// TODO : Test ¿ë ÇÔ¼ö.
	ENGINE_API void Create();

private:
	void CleanUp();

	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;

	ShaderConstants* pConstantBuffer_;

	Rasterizer* pRasterizer_;
};