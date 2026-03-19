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
	ITexture* pBricks_Color = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Color, 10)) DEBUG_BREAK();
	ITexture* pBricks_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Normal, 11)) DEBUG_BREAK();
	ITexture* pMaria_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Diffuse, 12)) DEBUG_BREAK();
	ITexture* pMaria_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Normal, 13)) DEBUG_BREAK();
	/*ITexture* pWukong_Torso_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWukong_Torso_Diffuse, 14)) DEBUG_BREAK();
	ITexture* pWukong_Torso_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWukong_Torso_Normal, 15)) DEBUG_BREAK();*/
	ITexture* pWerewolf_Body_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Albedo, 16)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Normal, 17)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Albedo, 18)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Normal, 19)) DEBUG_BREAK();


	MaterialDesc lightPassMaterialDesc;
	lightPassMaterialDesc.shaderType = E_SHADER_PRESET::LIGHT;
	lightPassMaterialDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	lightPassMaterialDesc.blendState = E_BLEND_PRESET::ADDITIVE_BLEND;
	lightPassMaterialDesc.textureNum = 4;
	lightPassMaterialDesc.shineness = 0.5f;
	lightPassMaterialDesc.specularColor = { 0.8f, 0.8f, 0.8f };
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

	MaterialDesc matDesc1;
	matDesc1.shaderType = E_SHADER_PRESET::STATIC_MESH;
	matDesc1.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	matDesc1.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc1.textureNum = 2;
	matDesc1.shineness = 0.7f;
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


	//MaterialDesc matDesc3;
	//matDesc3.shaderType = E_SHADER_PRESET::StaticMesh;
	//matDesc3.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
	//matDesc3.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	//matDesc3.textureNum_ = 2;
	//matDesc3.shineness = 0.7f;
	//matDesc3.specularColor = { 0.7f, 0.7f, 0.7f };
	//IMaterial* pMat3 = CreateMaterial(matDesc3, 12);
	//pMat3->SetTextures(0, pWukong_Torso_Diffuse);
	//pMat3->SetTextures(1, pWhiteTexture);


	MaterialDesc matDesc4;
	matDesc4.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	matDesc4.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	matDesc4.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc4.textureNum = 2;
	matDesc4.shineness = 0.7f;
	matDesc4.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat4 = CreateMaterial(matDesc4, 13);
	pMat4->SetTextures(0, pWerewolf_Body_Albedo);
	pMat4->SetTextures(1, pWerewolf_Body_Normal);


	MaterialDesc matDesc5;
	matDesc5.shaderType = E_SHADER_PRESET::SKINNED_MESH;
	matDesc5.samplerState = E_SAMPLER_PRESET::LINEAR_WARP;
	matDesc5.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	matDesc5.textureNum = 2;
	matDesc5.shineness = 0.7f;
	matDesc5.specularColor = { 0.7f, 0.7f, 0.7f };
	IMaterial* pMat5 = CreateMaterial(matDesc5, 14);
	pMat5->SetTextures(0, pWerewolf_Fur_Albedo);
	pMat5->SetTextures(1, pWerewolf_Fur_Normal);
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
