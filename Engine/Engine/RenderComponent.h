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

	// TODO 나중에 컴포넌트 구조 만들면 ActorComponent로 빼야함
	//Transform GetWorldTransform() const;

	// TODO 나중에 컴포넌트 구조 만들면 ActorComponent로 빼야함
	//Transform& GetRelativeTransform() const;

	// TODO : Test 용 함수.
	ENGINE_API void Create();

private:
	void CleanUp();

	Actor* pOwner_;

	// TODO 나중에 컴포넌트 구조에서 ActorComponent로 빼야함.
	//Transform* pRelativeTransform_;

	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;

	ShaderConstants* pConstantBuffer_;

	Rasterizer* pRasterizer_;
};