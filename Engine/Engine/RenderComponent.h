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

	//ENGINE_API void Create(uint16_t meshTag, uint16_t materialTag, uint16_t albedoTexTag, uint16_t normalTexTag);
	//ENGINE_API void Create(uint16_t meshTag, uint16_t materialTag, uint16_t albedoTexTag, uint16_t normalTexTag);
	ENGINE_API void Setting
	(
		uint16_t meshTag,
		uint16_t materialTag,
		E_SAMPLER_PRESET samplerState,
		E_DEPTH_PRESET depthState,
		E_BLEND_PRESET blendState,
		E_RASTERIZER_PRESET rasterizerMode
	);

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

	IPSO* pPSO_;
	IMesh* pMesh_;
	IMaterial* pMaterial_;

	/*uint16_t meshTag_;
	uint16_t materialTag_;
	E_SAMPLER_PRESET samplerState_;
	E_DEPTH_PRESET depthState_;
	E_BLEND_PRESET blendState_;
	E_RASTERIZER_PRESET rasterizerMode_;*/



	//IMesh* pMesh_;
	//IMaterial* pMaterial_;

	//ITexture* pTextureColor_;
	//ITexture* pTextureNormal_;
};

