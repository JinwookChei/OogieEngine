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

	ENGINE_API void Create();

private:
	void CleanUp();

	Actor* pOwner_;

	Mesh* pMesh_;

	Material* pMaterial_;

	InputLayout* pInputLayout_;

	ShaderConstants* pConstantBuffer_;

	Rasterizer* pRasterizer_;
};