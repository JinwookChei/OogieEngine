#include "stdafx.h"
#include "MaterialManager.h"


MaterialManager::MaterialManager()
	: materialTable_(32, 32)
{
}

MaterialManager::~MaterialManager()
{
	CleanUp();
}

MaterialManager* MaterialManager::Instance()
{
	return GMaterialManager;
}

void MaterialManager::TestLoad()
{
	ITexture* pWhiteTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWhiteTexture, "T_White", 7)) DEBUG_BREAK();
	ITexture* pDefaultNormalTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pDefaultNormalTexture, "T_DefaultNormal", 15)) DEBUG_BREAK();
	ITexture* pTransparentTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pTransparentTexture, "T_Transparent", 13)) DEBUG_BREAK();
	ITexture* pBricks_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Albedo, "T_Bricks_Albedo", 15)) DEBUG_BREAK();
	ITexture* pBricks_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Normal, "T_Bricks_Normal", 15)) DEBUG_BREAK();
	ITexture* pMaria_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Albedo, "T_Maria_Albedo", 14)) DEBUG_BREAK();
	ITexture* pMaria_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Normal, "T_Maria_Normal", 14)) DEBUG_BREAK();
	ITexture* pLightBulb_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pLightBulb_Albedo, "T_LightBulb_Albedo", 18)) DEBUG_BREAK();
	ITexture* pLightBulb_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pLightBulb_Normal, "T_LightBulb_Normal", 18)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Albedo, "T_Werewolf_Body_Albedo", 22)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Normal, "T_Werewolf_Body_Normal", 22)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Albedo, "T_Werewolf_Fur_Albedo", 21)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Normal, "T_Werewolf_Fur_Normal", 21)) DEBUG_BREAK();
	ITexture* pTerrain_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pTerrain_Albedo, "T_Terrain_Albedo", 16)) DEBUG_BREAK();
	ITexture* pTerrain_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pTerrain_Normal, "T_Terrain_Normal", 16)) DEBUG_BREAK();

	// HouseTexture
	ITexture* pHouse_Decals_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_Decals_Albedo, "T_House_Decals_Albedo", 21)) DEBUG_BREAK();
	ITexture* pHouse_Decals_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_Decals_Normal, "T_House_Decals_Normal", 21)) DEBUG_BREAK();
	ITexture* pHouse_WoodTrims_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WoodTrims_Albedo, "T_House_WoodTrims_Albedo", 24)) DEBUG_BREAK();
	ITexture* pHouse_WoodTrims_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WoodTrims_Normal, "T_House_WoodTrims_Normal", 24)) DEBUG_BREAK();
	ITexture* pHouse_RoofTile_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_RoofTile_Albedo, "T_House_RoofTile_Albedo", 23)) DEBUG_BREAK();
	ITexture* pHouse_RoofTile_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_RoofTile_Normal, "T_House_RoofTile_Normal", 23)) DEBUG_BREAK();
	ITexture* pHouse_WoodEnd_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WoodEnd_Albedo, "T_House_WoodEnd_Albedo", 22)) DEBUG_BREAK();
	ITexture* pHouse_WoodEnd_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WoodEnd_Normal, "T_House_WoodEnd_Normal", 22)) DEBUG_BREAK();
	ITexture* pHouse_WallPlaster_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WallPlaster_Albedo, "T_House_WallPlaster_Albedo", 26)) DEBUG_BREAK();
	ITexture* pHouse_WallPlaster_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WallPlaster_Normal, "T_House_WallPlaster_Normal", 26)) DEBUG_BREAK();
	ITexture* pHouse_Metal_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_Metal_Albedo, "T_House_Metal_Albedo", 20)) DEBUG_BREAK();
	ITexture* pHouse_Metal_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_Metal_Normal, "T_House_Metal_Normal", 20)) DEBUG_BREAK();
	ITexture* pHouse_WallStone_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WallStone_Albedo, "T_House_WallStone_Albedo", 24)) DEBUG_BREAK();
	ITexture* pHouse_WallStone_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_WallStone_Normal, "T_House_WallStone_Normal", 24)) DEBUG_BREAK();
	ITexture* pHouse_GlassWindow_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_GlassWindow_Albedo, "T_House_GlassWindow_Albedo", 26)) DEBUG_BREAK();
	ITexture* pHouse_GlassWindow_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pHouse_GlassWindow_Normal, "T_House_GlassWindow_Albedo", 26)) DEBUG_BREAK();


	MaterialDesc lightPassMaterialDesc;
	lightPassMaterialDesc.shaderType = E_SHADER_PRESET::LIGHT;
	lightPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	lightPassMaterialDesc.blendState = E_BLEND_PRESET::ADDITIVE_BLEND;
	lightPassMaterialDesc.textureNum = 4;
	IMaterial* pLightPassMaterial = CreateMaterial(lightPassMaterialDesc, "MT_LightPass", 12);

	MaterialDesc particleComputeMaterialDesc;
	particleComputeMaterialDesc.shaderType = E_SHADER_PRESET::PARTICLE_COMPUTE;
	particleComputeMaterialDesc.samplerState = E_SAMPLER_PRESET::DISABLE;
	particleComputeMaterialDesc.blendState = E_BLEND_PRESET::DISABLE;
	particleComputeMaterialDesc.textureNum = 0;
	IMaterial* pParticleComputeMaterial = CreateMaterial(particleComputeMaterialDesc, "MT_Particle_Compute", 19);

	MaterialDesc particlePassMaterialDesc;
	particlePassMaterialDesc.shaderType = E_SHADER_PRESET::PARTICLE_RENDER;
	particlePassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	particlePassMaterialDesc.blendState = E_BLEND_PRESET::ALPHA_BLEND;
	particlePassMaterialDesc.textureNum = 1;
	IMaterial* pParticlePassMaterial = CreateMaterial(particlePassMaterialDesc, "MT_ParticlePass", 15);
	pParticlePassMaterial->SetTextures(0, pWhiteTexture);

	MaterialDesc blitPassMaterialDesc;
	blitPassMaterialDesc.blendState = E_BLEND_PRESET::ALPHA_BLEND;
	blitPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	blitPassMaterialDesc.shaderType = E_SHADER_PRESET::BLIT;
	blitPassMaterialDesc.textureNum = 1;
	IMaterial* pBlitPassMaterial = CreateMaterial(blitPassMaterialDesc, "MT_BlitPass", 11);

	MaterialDesc ambientPassMaterialDesc;
	ambientPassMaterialDesc.shaderType = E_SHADER_PRESET::AMBIENT;
	ambientPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	ambientPassMaterialDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	ambientPassMaterialDesc.textureNum = 1;
	IMaterial* pAmbientPassMaterial = CreateMaterial(ambientPassMaterialDesc, "MT_AmbientPass", 14);

	MaterialDesc lightBulbDesc;
	lightBulbDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	lightBulbDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	lightBulbDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	lightBulbDesc.textureNum = 2;
	lightBulbDesc.shineness = 0.7f;
	lightBulbDesc.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pLightBulbMaterial = CreateMaterial(lightBulbDesc, "MT_LightBulb", 12);
	pLightBulbMaterial->SetTextures(0, pLightBulb_Albedo);
	pLightBulbMaterial->SetTextures(1, pLightBulb_Normal);

	MaterialDesc bricksDesc;
	bricksDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	bricksDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	bricksDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	bricksDesc.textureNum = 2;
	bricksDesc.shineness = 0.3f;
	bricksDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pBricksMaterial = CreateMaterial(bricksDesc, "MT_Bricks", 9);
	pBricksMaterial->SetTextures(0, pBricks_Albedo);
	pBricksMaterial->SetTextures(1, pBricks_Normal);

	MaterialDesc mariaDesc;
	mariaDesc.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	mariaDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	mariaDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	mariaDesc.textureNum = 2;
	mariaDesc.shineness = 0.7f;
	mariaDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMariaMaterial = CreateMaterial(mariaDesc, "MT_Maria", 8);
	pMariaMaterial->SetTextures(0, pMaria_Albedo);
	pMariaMaterial->SetTextures(1, pMaria_Normal);

	MaterialDesc werewolfBodyDesc;
	werewolfBodyDesc.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	werewolfBodyDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	werewolfBodyDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	werewolfBodyDesc.textureNum = 2;
	werewolfBodyDesc.shineness = 0.2f;
	werewolfBodyDesc.specularColor = { 0.5f, 0.5f, 0.5f };
	IMaterial* pWerewolfBodyMaterial = CreateMaterial(werewolfBodyDesc, "MT_Werewolf_Body", 16);
	pWerewolfBodyMaterial->SetTextures(0, pWerewolf_Body_Albedo);
	pWerewolfBodyMaterial->SetTextures(1, pWerewolf_Body_Normal);

	MaterialDesc werewolfFurDesc;
	werewolfFurDesc.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	werewolfFurDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	werewolfFurDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	werewolfFurDesc.textureNum = 2;
	werewolfFurDesc.shineness = 0.5f;
	werewolfFurDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pWerewolfFurMaterial = CreateMaterial(werewolfFurDesc, "MT_Werewolf_Fur", 15);
	pWerewolfFurMaterial->SetTextures(0, pWerewolf_Fur_Albedo);
	pWerewolfFurMaterial->SetTextures(1, pWerewolf_Fur_Normal);

	MaterialDesc testBricksDesc;
	testBricksDesc.shaderType = E_SHADER_PRESET::TEST_MESH;
	testBricksDesc.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	testBricksDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	testBricksDesc.textureNum = 2;
	testBricksDesc.shineness = 0.5f;
	testBricksDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pTestBricksMaterial = CreateMaterial(testBricksDesc, "MT_TestBricks", 13);
	pTestBricksMaterial->SetTextures(0, pBricks_Albedo);
	pTestBricksMaterial->SetTextures(1, pBricks_Normal);

	MaterialDesc terrainDesc;
	terrainDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	terrainDesc.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	terrainDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	terrainDesc.textureNum = 2;
	terrainDesc.shineness = 0.1f;
	terrainDesc.specularColor = { 0.2f, 0.2f, 0.2f };
	IMaterial* pTerrainMaterial = CreateMaterial(terrainDesc, "MT_Terrain", 10);
	pTerrainMaterial->SetTextures(0, pTerrain_Albedo);
	pTerrainMaterial->SetTextures(1, pTerrain_Normal);

	MaterialDesc staticColorDesc;
	staticColorDesc.shaderType = E_SHADER_PRESET::STATIC_COLOR_MESH;
	staticColorDesc.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	staticColorDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	staticColorDesc.textureNum = 0;
	staticColorDesc.shineness = 0.5f;
	staticColorDesc.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pStaticColorMaterial = CreateMaterial(staticColorDesc, "MT_StaticColor", 14);

	// House
	// い Decal
	MaterialDesc houseDecalsMatDesc;
	houseDecalsMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseDecalsMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseDecalsMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseDecalsMatDesc.textureNum = 2;
	houseDecalsMatDesc.shineness = 0.2f;
	houseDecalsMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseDecalsMat = CreateMaterial(houseDecalsMatDesc, "MT_House_Decals", 15);
	pHouseDecalsMat->SetTextures(0, pHouse_Decals_Albedo);
	pHouseDecalsMat->SetTextures(1, pHouse_Decals_Normal);
	// い WoodTrim
	MaterialDesc houseWoodTrimsMatDesc;
	houseWoodTrimsMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWoodTrimsMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWoodTrimsMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWoodTrimsMatDesc.textureNum = 2;
	houseWoodTrimsMatDesc.shineness = 0.2f;
	houseWoodTrimsMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseWoodTrimsMat = CreateMaterial(houseWoodTrimsMatDesc, "MT_House_WoodTrims", 18);
	pHouseWoodTrimsMat->SetTextures(0, pHouse_WoodTrims_Albedo);
	pHouseWoodTrimsMat->SetTextures(1, pHouse_WoodTrims_Normal);
	// い RoofTile
	MaterialDesc houseRoofTileMatDesc;
	houseRoofTileMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseRoofTileMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseRoofTileMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseRoofTileMatDesc.textureNum = 2;
	houseRoofTileMatDesc.shineness = 0.2f;
	houseRoofTileMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseRoofTileMat = CreateMaterial(houseRoofTileMatDesc, "MT_House_RoofTile", 17);
	pHouseRoofTileMat->SetTextures(0, pHouse_RoofTile_Albedo);
	pHouseRoofTileMat->SetTextures(1, pHouse_RoofTile_Normal);
	// い WoodEnd
	MaterialDesc houseWoodEndMatDesc;
	houseWoodEndMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWoodEndMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseWoodEndMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWoodEndMatDesc.textureNum = 2;
	houseWoodEndMatDesc.shineness = 0.2f;
	houseWoodEndMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseWoodEndMat = CreateMaterial(houseWoodEndMatDesc, "MT_House_WoodEnd", 16);
	pHouseWoodEndMat->SetTextures(0, pHouse_WoodEnd_Albedo);
	pHouseWoodEndMat->SetTextures(1, pHouse_WoodEnd_Normal);
	// い WallPlaster
	MaterialDesc houseWallPlasterMatDesc;
	houseWallPlasterMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWallPlasterMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWallPlasterMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWallPlasterMatDesc.textureNum = 2;
	houseWallPlasterMatDesc.shineness = 0.15f;
	houseWallPlasterMatDesc.specularColor = { 0.6f, 0.6f, 0.6f };
	IMaterial* pHouseWallPlasterMat = CreateMaterial(houseWallPlasterMatDesc, "MT_House_WallPlaster", 20);
	pHouseWallPlasterMat->SetTextures(0, pHouse_WallPlaster_Albedo);
	pHouseWallPlasterMat->SetTextures(1, pHouse_WallPlaster_Normal);
	// い Metal
	MaterialDesc houseMetalMatDesc;
	houseMetalMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseMetalMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseMetalMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseMetalMatDesc.textureNum = 2;
	houseMetalMatDesc.shineness = 1.0f;
	houseMetalMatDesc.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pHouseMetalMat = CreateMaterial(houseMetalMatDesc, "MT_House_Metal", 14);
	pHouseMetalMat->SetTextures(0, pHouse_Metal_Albedo);
	pHouseMetalMat->SetTextures(1, pHouse_Metal_Normal);
	// い WallStone
	MaterialDesc houseWallStoneMatDesc;
	houseWallStoneMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWallStoneMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWallStoneMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWallStoneMatDesc.textureNum = 2;
	houseWallStoneMatDesc.shineness = 0.2f;
	houseWallStoneMatDesc.specularColor = { 0.2f, 0.2f, 0.2f };
	IMaterial* pHouseWallStoneMat = CreateMaterial(houseWallStoneMatDesc, "MT_House_WallStone", 18);
	pHouseWallStoneMat->SetTextures(0, pHouse_WallStone_Albedo);
	pHouseWallStoneMat->SetTextures(1, pHouse_WallStone_Normal);
	// い GlassWindow
	MaterialDesc houseGlassWindowMatDesc;
	houseGlassWindowMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseGlassWindowMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseGlassWindowMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseGlassWindowMatDesc.textureNum = 2;
	houseGlassWindowMatDesc.shineness = 0.6f;
	houseGlassWindowMatDesc.specularColor = { 0.9f, 0.9f, 0.9f };
	IMaterial* pHouseGlassWindowMat = CreateMaterial(houseGlassWindowMatDesc, "MT_House_GlassWindow", 20);
	pHouseGlassWindowMat->SetTextures(0, pHouse_GlassWindow_Albedo);
	pHouseGlassWindowMat->SetTextures(1, pHouse_GlassWindow_Normal);
	// い Transparent
	MaterialDesc houseTransparentMatDesc;
	houseTransparentMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseTransparentMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseTransparentMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseTransparentMatDesc.textureNum = 2;
	houseTransparentMatDesc.shineness = 0.3f;
	houseTransparentMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseTransparentMat = CreateMaterial(houseTransparentMatDesc, "MT_House_Transparent", 20);
	pHouseTransparentMat->SetTextures(0, pTransparentTexture);
	pHouseTransparentMat->SetTextures(1, pDefaultNormalTexture);
}

IMaterial* MaterialManager::CreateMaterial(const MaterialDesc& desc, const char* materialKey, unsigned int keySize)
{
	IMaterial* pNewMaterial = Renderer::GetFactory()->CreateMaterial(desc);
	materialTable_.Insert(pNewMaterial, materialKey, keySize);
	return pNewMaterial;
}

bool MaterialManager::GetMaterial(IMaterial** ppOutMaterial, const char* materialKey, unsigned int keySize)
{
	unsigned int searchedCount = 0;
	void* pTmpMaterial = nullptr;
	if (false == materialTable_.Search((void**)&pTmpMaterial, &searchedCount, 8, materialKey, keySize))
	{
		Assert("Material Search is Fail!!");
		return false;
	}
	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}

	*ppOutMaterial = static_cast<IMaterial*>(pTmpMaterial);
	return true;
}

void MaterialManager::CleanUp()
{
	HashTableIterator materialTableEnd = materialTable_.end();
	for (HashTableIterator iter = materialTable_.begin(); iter != materialTableEnd;)
	{
		IMaterial* pCurMaterial = static_cast<IMaterial*>(*iter);
		if (nullptr != pCurMaterial)
		{
			pCurMaterial->Release();
			pCurMaterial = nullptr;
		}
		iter = materialTable_.Delete(iter);
	}
}
