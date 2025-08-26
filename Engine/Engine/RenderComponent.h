#pragma once


class Mesh;
class Material;
class InputLayout;

class RenderComponent
{
public:
	RenderComponent();

	virtual ~RenderComponent();

	void Setting();

	void Draw();	

	// TODO : Test ¿ë ÇÔ¼ö.
	//void Create();

private:
	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;
};