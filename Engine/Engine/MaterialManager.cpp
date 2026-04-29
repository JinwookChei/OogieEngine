#include "stdafx.h"
#include "MaterialManager.h"


MaterialManager::MaterialManager()
	: materialTable_(16, 8)
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
	if (false == TextureManager::Instance()->GetTexture(&pWhiteTexture, 0)) DEBUG_BREAK();
	ITexture* pDefaultNormalTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pDefaultNormalTexture, 1)) DEBUG_BREAK();
	ITexture* pTransparentTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pTransparentTexture, 2)) DEBUG_BREAK();
	ITexture* pBricks_Color = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Color, 10)) DEBUG_BREAK();
	ITexture* pBricks_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Normal, 11)) DEBUG_BREAK();
	ITexture* pMaria_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Diffuse, 12)) DEBUG_BREAK();
	ITexture* pMaria_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Normal, 13)) DEBUG_BREAK();
	ITexture* pLightBulb_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pLightBulb_Diffuse, 14)) DEBUG_BREAK();
	ITexture* pLightBulb_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pLightBulb_Normal, 15)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Albedo, 16)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Normal, 17)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Albedo, 18)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Normal, 19)) DEBUG_BREAK();
	ITexture* grassGroundColor_Color = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&grassGroundColor_Color, 20)) DEBUG_BREAK();
	ITexture* grassGroundColor_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&grassGroundColor_Normal, 21)) DEBUG_BREAK();

	// HouseTexture
	ITexture* houseDecals_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseDecals_Albedo, 100)) DEBUG_BREAK();
	ITexture* houseDecals_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseDecals_Normal, 101)) DEBUG_BREAK();
	ITexture* houseWoodTrims_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWoodTrims_Albedo, 102)) DEBUG_BREAK();
	ITexture* houseWoodTrims_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWoodTrims_Normal, 103)) DEBUG_BREAK();
	ITexture* houseRoofTile_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseRoofTile_Albedo, 104)) DEBUG_BREAK();
	ITexture* houseRoofTile_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseRoofTile_Normal, 105)) DEBUG_BREAK();
	ITexture* houseWoodEnd_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWoodEnd_Albedo, 106)) DEBUG_BREAK();
	ITexture* houseWoodEnd_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWoodEnd_Normal, 107)) DEBUG_BREAK();
	ITexture* houseWallPlaster_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWallPlaster_Albedo, 108)) DEBUG_BREAK();
	ITexture* houseWallPlaster_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWallPlaster_Normal, 109)) DEBUG_BREAK();
	ITexture* houseMetal_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseMetal_Albedo, 110)) DEBUG_BREAK();
	ITexture* houseMetal_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseMetal_Normal, 111)) DEBUG_BREAK();
	ITexture* houseWallStone_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWallStone_Albedo, 112)) DEBUG_BREAK();
	ITexture* houseWallStone_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseWallStone_Normal, 113)) DEBUG_BREAK();
	ITexture* houseGlassWindow_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseGlassWindow_Albedo, 114)) DEBUG_BREAK();
	ITexture* houseGlassWindow_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&houseGlassWindow_Normal, 115)) DEBUG_BREAK();


	MaterialDesc lightPassMaterialDesc;
	lightPassMaterialDesc.shaderType = E_SHADER_PRESET::LIGHT;
	lightPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	lightPassMaterialDesc.blendState = E_BLEND_PRESET::ADDITIVE_BLEND;
	lightPassMaterialDesc.textureNum = 4;
	IMaterial* pLightMaterial = CreateMaterial(lightPassMaterialDesc, 0);

	MaterialDesc particleComputeMaterialDesc;
	particleComputeMaterialDesc.shaderType = E_SHADER_PRESET::PARTICLE_COMPUTE;
	particleComputeMaterialDesc.samplerState = E_SAMPLER_PRESET::DISABLE;
	particleComputeMaterialDesc.blendState = E_BLEND_PRESET::DISABLE;
	particleComputeMaterialDesc.textureNum = 0;
	IMaterial* pParticleComputeMaterial = CreateMaterial(particleComputeMaterialDesc, 1);

	MaterialDesc particlePassMaterialDesc;
	particlePassMaterialDesc.shaderType = E_SHADER_PRESET::PARTICLE_RENDER;
	particlePassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	particlePassMaterialDesc.blendState = E_BLEND_PRESET::ALPHA_BLEND;
	particlePassMaterialDesc.textureNum = 1;
	IMaterial* pParticleRenderMaterial = CreateMaterial(particlePassMaterialDesc, 2);
	pParticleRenderMaterial->SetTextures(0, pWhiteTexture);

	MaterialDesc blitPassMaterialDesc;
	blitPassMaterialDesc.blendState = E_BLEND_PRESET::ALPHA_BLEND;
	blitPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	blitPassMaterialDesc.shaderType = E_SHADER_PRESET::BLIT;
	blitPassMaterialDesc.textureNum = 1;
	IMaterial* pMat = CreateMaterial(blitPassMaterialDesc, 3);

	MaterialDesc ambientPassMaterialDesc;
	ambientPassMaterialDesc.shaderType = E_SHADER_PRESET::AMBIENT;
	ambientPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	ambientPassMaterialDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	ambientPassMaterialDesc.textureNum = 1;
	IMaterial* pAmbientMaterial = CreateMaterial(ambientPassMaterialDesc, 4);

	MaterialDesc matDesc0;
	matDesc0.shaderType = E_SHADER_PRESET::STATIC_MESH;
	matDesc0.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	matDesc0.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc0.textureNum = 2;
	matDesc0.shineness = 0.7f;
	matDesc0.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pMat0 = CreateMaterial(matDesc0, 9);
	pMat0->SetTextures(0, pLightBulb_Diffuse);
	pMat0->SetTextures(1, pLightBulb_Normal);

	MaterialDesc matDesc1;
	matDesc1.shaderType = E_SHADER_PRESET::STATIC_MESH;
	matDesc1.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	matDesc1.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc1.textureNum = 2;
	matDesc1.shineness = 0.3f;
	matDesc1.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat1 = CreateMaterial(matDesc1, 10);
	pMat1->SetTextures(0, pBricks_Color);
	pMat1->SetTextures(1, pBricks_Normal);

	MaterialDesc matDesc2;
	matDesc2.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	matDesc2.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	matDesc2.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc2.textureNum = 2;
	matDesc2.shineness = 0.7f;
	matDesc2.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat2 = CreateMaterial(matDesc2, 11);
	pMat2->SetTextures(0, pMaria_Diffuse);
	pMat2->SetTextures(1, pMaria_Normal);

	MaterialDesc matDesc4;
	matDesc4.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	matDesc4.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	matDesc4.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc4.textureNum = 2;
	matDesc4.shineness = 0.2f;
	matDesc4.specularColor = { 0.5f, 0.5f, 0.5f };
	IMaterial* pMat4 = CreateMaterial(matDesc4, 13);
	pMat4->SetTextures(0, pWerewolf_Body_Albedo);
	pMat4->SetTextures(1, pWerewolf_Body_Normal);

	MaterialDesc matDesc5;
	matDesc5.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	matDesc5.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	matDesc5.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc5.textureNum = 2;
	matDesc5.shineness = 0.5f;
	matDesc5.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat5 = CreateMaterial(matDesc5, 14);
	pMat5->SetTextures(0, pWerewolf_Fur_Albedo);
	pMat5->SetTextures(1, pWerewolf_Fur_Normal);

	MaterialDesc matDesc6;
	matDesc6.shaderType = E_SHADER_PRESET::TEST_MESH;
	matDesc6.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	matDesc6.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc6.textureNum = 2;
	matDesc6.shineness = 0.5f;
	matDesc6.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat6 = CreateMaterial(matDesc6, 15);
	pMat6->SetTextures(0, pBricks_Color);
	pMat6->SetTextures(1, pBricks_Normal);

	MaterialDesc matDesc7;
	matDesc7.shaderType = E_SHADER_PRESET::STATIC_MESH;
	matDesc7.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	matDesc7.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc7.textureNum = 2;
	matDesc7.shineness = 0.1f;
	matDesc7.specularColor = { 0.2f, 0.2f, 0.2f };
	IMaterial* pMat7 = CreateMaterial(matDesc7, 16);
	pMat7->SetTextures(0, grassGroundColor_Color);
	pMat7->SetTextures(1, grassGroundColor_Normal);

	MaterialDesc matDesc8;
	matDesc8.shaderType = E_SHADER_PRESET::STATIC_COLOR_MESH;
	matDesc8.samplerState = E_SAMPLER_PRESET::ANISOTROPIC_WARP;
	matDesc8.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc8.textureNum = 0;
	matDesc8.shineness = 0.5f;
	matDesc8.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pMat8 = CreateMaterial(matDesc8, 17);

	// House
	// い Decal
	MaterialDesc houseDecalMatDesc;
	houseDecalMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseDecalMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseDecalMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseDecalMatDesc.textureNum = 2;
	houseDecalMatDesc.shineness = 0.2f;
	houseDecalMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseDecalMat = CreateMaterial(houseDecalMatDesc, 100);
	pHouseDecalMat->SetTextures(0, houseDecals_Albedo);
	pHouseDecalMat->SetTextures(1, houseDecals_Normal);
	// い WoodTrim
	MaterialDesc houseWoodTrimMatDesc;
	houseWoodTrimMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWoodTrimMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWoodTrimMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWoodTrimMatDesc.textureNum = 2;
	houseWoodTrimMatDesc.shineness = 0.2f;
	houseWoodTrimMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseWoodTrimMat = CreateMaterial(houseWoodTrimMatDesc, 101);
	pHouseWoodTrimMat->SetTextures(0, houseWoodTrims_Albedo);
	pHouseWoodTrimMat->SetTextures(1, houseWoodTrims_Normal);
	// い RoofTile
	MaterialDesc houseRoofTileMatDesc;
	houseRoofTileMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseRoofTileMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseRoofTileMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseRoofTileMatDesc.textureNum = 2;
	houseRoofTileMatDesc.shineness = 0.2f;
	houseRoofTileMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseRoofTileMat = CreateMaterial(houseRoofTileMatDesc, 102);
	pHouseRoofTileMat->SetTextures(0, houseRoofTile_Albedo);
	pHouseRoofTileMat->SetTextures(1, houseRoofTile_Normal);
	// い WoodEnd
	MaterialDesc houseWoodEndMatDesc;
	houseWoodEndMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWoodEndMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseWoodEndMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWoodEndMatDesc.textureNum = 2;
	houseWoodEndMatDesc.shineness = 0.2f;
	houseWoodEndMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseWoodEndMat = CreateMaterial(houseWoodEndMatDesc, 103);
	pHouseWoodEndMat->SetTextures(0, houseWoodEnd_Albedo);
	pHouseWoodEndMat->SetTextures(1, houseWoodEnd_Normal);
	// い WallPlaster
	MaterialDesc houseWallPlasterMatDesc;
	houseWallPlasterMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWallPlasterMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWallPlasterMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWallPlasterMatDesc.textureNum = 2;
	houseWallPlasterMatDesc.shineness = 0.15f;
	houseWallPlasterMatDesc.specularColor = { 0.6f, 0.6f, 0.6f };
	IMaterial* pHouseWallPlasterMat = CreateMaterial(houseWallPlasterMatDesc, 104);
	pHouseWallPlasterMat->SetTextures(0, houseWallPlaster_Albedo);
	pHouseWallPlasterMat->SetTextures(1, houseWallPlaster_Normal);
	// い Metal
	MaterialDesc houseMetalMatDesc;
	houseMetalMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseMetalMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseMetalMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseMetalMatDesc.textureNum = 2;
	houseMetalMatDesc.shineness = 1.0f;
	houseMetalMatDesc.specularColor = { 1.0f, 1.0f, 1.0f };
	IMaterial* pHouseMetalMat = CreateMaterial(houseMetalMatDesc, 105);
	pHouseMetalMat->SetTextures(0, houseMetal_Albedo);
	pHouseMetalMat->SetTextures(1, houseMetal_Normal);
	// い WallStone
	MaterialDesc houseWallStoneMatDesc;
	houseWallStoneMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseWallStoneMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseWallStoneMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseWallStoneMatDesc.textureNum = 2;
	houseWallStoneMatDesc.shineness = 0.2f;
	houseWallStoneMatDesc.specularColor = { 0.2f, 0.2f, 0.2f };
	IMaterial* pHouseWallStoneMat = CreateMaterial(houseWallStoneMatDesc, 106);
	pHouseWallStoneMat->SetTextures(0, houseWallStone_Albedo);
	pHouseWallStoneMat->SetTextures(1, houseWallStone_Normal);
	// い GlassWindow
	MaterialDesc houseGlassWindowMatDesc;
	houseGlassWindowMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseGlassWindowMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	houseGlassWindowMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseGlassWindowMatDesc.textureNum = 2;
	houseGlassWindowMatDesc.shineness = 0.6f;
	houseGlassWindowMatDesc.specularColor = { 0.9f, 0.9f, 0.9f };
	IMaterial* pHouseGlassWindowMat = CreateMaterial(houseGlassWindowMatDesc, 107);
	pHouseGlassWindowMat->SetTextures(0, houseGlassWindow_Albedo);
	pHouseGlassWindowMat->SetTextures(1, houseGlassWindow_Normal);
	// い Transparent
	MaterialDesc houseTransparentMatDesc;
	houseTransparentMatDesc.shaderType = E_SHADER_PRESET::STATIC_MESH;
	houseTransparentMatDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	houseTransparentMatDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	houseTransparentMatDesc.textureNum = 2;
	houseTransparentMatDesc.shineness = 0.3f;
	houseTransparentMatDesc.specularColor = { 0.1f, 0.1f, 0.1f };
	IMaterial* pHouseTransparentMat = CreateMaterial(houseTransparentMatDesc, 108);
	pHouseTransparentMat->SetTextures(0, pTransparentTexture);
	pHouseTransparentMat->SetTextures(1, pDefaultNormalTexture);
}

IMaterial* MaterialManager::CreateMaterial(const MaterialDesc& desc, unsigned long long materialTag)
{
	IMaterial* pNewMaterial = Renderer::GetFactory()->CreateMaterial(desc);
	materialTable_.Insert(pNewMaterial, &materialTag, 8);
	return pNewMaterial;
}

bool MaterialManager::GetMaterial(IMaterial** ppOutMaterial, unsigned long long materialTag)
{
	unsigned int searchedCount = 0;
	void* pTmpMaterial = nullptr;

	if (false == materialTable_.Search((void**)&pTmpMaterial, &searchedCount, 8, &materialTag, 8))
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
