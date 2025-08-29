#pragma once

class Actor;
class Transform;
class Mesh;
class Material;
class Shader;
class InputLayout;
class ShaderConstants;
class Rasterizer;

class RenderComponent
{
public:
	ENGINE_API RenderComponent(Actor* pOwner);

	ENGINE_API virtual ~RenderComponent();

	ENGINE_API void Render();	

	// TODO ���߿� ������Ʈ ���� ����� ActorComponent�� ������
	//Transform GetWorldTransform() const;

	// TODO ���߿� ������Ʈ ���� ����� ActorComponent�� ������
	//Transform& GetRelativeTransform() const;

	// TODO : Test �� �Լ�.
	ENGINE_API void Create();

private:
	void CleanUp();

	Actor* pOwner_;

	// TODO ���߿� ������Ʈ �������� ActorComponent�� ������.
	//Transform* pRelativeTransform_;

	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;

	ShaderConstants* pConstantBuffer_;

	Rasterizer* pRasterizer_;
};