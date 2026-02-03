#pragma once

class Actor;
class Transform;
class Mesh;
class Material;
class Shader;
class InputLayout;
class ShaderConstants;
class Rasterizer;

//enum class E_MESH_TYPE
//{
//	SPHERE = 0,
//	CUBE,
//	WUKONG
//};

class RenderComponent
{
public:
	ENGINE_API RenderComponent(Actor* pOwner);

	ENGINE_API virtual ~RenderComponent();

	ENGINE_API void Render();

	ENGINE_API void Create(uint16_t meshTag, uint16_t materialTag, uint16_t albedoTexTag, uint16_t normalTexTag);

	IMesh* GetMesh() const;

// MeshLoad Delegate
public:
	using MeshLoadedDelegate = std::function<void(IMesh*)>;
	void BindOnMeshLoaded(MeshLoadedDelegate callback);

private:
	void BroadcastOnMeshLoaded();
	std::vector<MeshLoadedDelegate> OnMeshLoaded_;
// MeshLoad Delegate End	

private:
	void CleanUp();

	Actor* pOwner_;

	IMesh* pMesh_;
	IMaterial* pMaterial_;

	ITexture* pTextureColor_;
	ITexture* pTextureNormal_;
};