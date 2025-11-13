#pragma once

class Actor;
class Transform;
class Mesh;
class Material;
class Shader;
class InputLayout;
class ShaderConstants;
class Rasterizer;

enum class E_MESH_TYPE
{
	SPHERE = 0,
	CUBE
};

class RenderComponent
{
public:
	ENGINE_API RenderComponent(Actor* pOwner);

	ENGINE_API virtual ~RenderComponent();

	ENGINE_API void Render();	

	ENGINE_API void Create(E_MESH_TYPE meshType);

private:
	void CleanUp();

	Actor* pOwner_;

	IMesh* pMesh_;

	IMaterial* pMaterial_;

	ITexture* pTextureColor_;
	ITexture* pTextureNormal_;

	IInputLayout* pInputLayout_;

	IRasterizer* pRasterizer_;
};