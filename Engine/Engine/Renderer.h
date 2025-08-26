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

	// TODO : Test �� �Լ�.
	//void Create();

private:
	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;
};