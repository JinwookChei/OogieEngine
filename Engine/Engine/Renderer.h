#pragma once


class Mesh;
class Material;
class InputLayout;

class Renderer
{
public:
	Renderer();

	virtual ~Renderer();

	void Setting();

	void Draw();	

	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;
};