#include "stdafx.h"
#include "MaterialManager.h"


MaterialManager::MaterialManager()
	: materialTable_(16,8)
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
	//MaterialDesc matDesc;
	////matDesc.VS = L"ObjectVS.cso";
	////matDesc.PS = L"DeferredObjectPS.cso";
	//matDesc.shineness = 0.7f;
	//matDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	//CreateMaterial(matDesc, 1);

	ITexture* pWhiteTexture = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWhiteTexture, 0)) DEBUG_BREAK();

	ITexture* pBricks_Color = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Color, 1)) DEBUG_BREAK();
	ITexture* pBricks_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pBricks_Normal, 2)) DEBUG_BREAK();

	ITexture* pMaria_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Diffuse, 3)) DEBUG_BREAK();
	ITexture* pMaria_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pMaria_Normal, 4)) DEBUG_BREAK();

	ITexture* pWukong_Torso_Diffuse = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWukong_Torso_Diffuse, 5)) DEBUG_BREAK();
	ITexture* pWukong_Torso_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWukong_Torso_Normal, 6)) DEBUG_BREAK();

	ITexture* pWerewolf_Body_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Albedo, 7)) DEBUG_BREAK();
	ITexture* pWerewolf_Body_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Body_Normal, 8)) DEBUG_BREAK();

	ITexture* pWerewolf_Fur_Albedo = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Albedo, 9)) DEBUG_BREAK();
	ITexture* pWerewolf_Fur_Normal = nullptr;
	if (false == TextureManager::Instance()->GetTexture(&pWerewolf_Fur_Normal, 10)) DEBUG_BREAK();


	MaterialDesc matDesc1;
	matDesc1.shaderType = E_SHADER_PRESET::DeferredSimple;
	matDesc1.textureCount_ = 2;
	matDesc1.ppTextures_ = new ITexture * [matDesc1.textureCount_];
	matDesc1.ppTextures_[0] = pBricks_Color;
	matDesc1.ppTextures_[1] = pBricks_Normal;
	matDesc1.shineness = 0.7f;
	matDesc1.specularColor = { 0.7f, 0.7f, 0.7f };
	CreateMaterial(matDesc1, 1);


	MaterialDesc matDesc2;
	matDesc2.shaderType = E_SHADER_PRESET::DeferredSimple;
	matDesc2.textureCount_ = 2;
	matDesc2.ppTextures_ = new ITexture * [matDesc2.textureCount_];
	matDesc2.ppTextures_[0] = pMaria_Diffuse;
	matDesc2.ppTextures_[1] = pMaria_Normal;
	matDesc2.shineness = 0.7f;
	matDesc2.specularColor = { 0.7f, 0.7f, 0.7f };
	CreateMaterial(matDesc2, 2);


	MaterialDesc matDesc3;
	matDesc3.shaderType = E_SHADER_PRESET::DeferredSimple;
	matDesc3.textureCount_ = 2;
	matDesc3.ppTextures_ = new ITexture * [matDesc3.textureCount_];
	matDesc3.ppTextures_[0] = pWukong_Torso_Diffuse;
	matDesc3.ppTextures_[1] = pWukong_Torso_Normal;
	matDesc3.shineness = 0.7f;
	matDesc3.specularColor = { 0.7f, 0.7f, 0.7f };
	CreateMaterial(matDesc3, 3);


	MaterialDesc matDesc4;
	matDesc4.shaderType = E_SHADER_PRESET::DeferredSimple;
	matDesc4.textureCount_ = 2;
	matDesc4.ppTextures_ = new ITexture * [matDesc4.textureCount_];
	matDesc4.ppTextures_[0] = pWerewolf_Body_Albedo;
	matDesc4.ppTextures_[1] = pWerewolf_Body_Normal;
	matDesc4.shineness = 0.7f;
	matDesc4.specularColor = { 0.7f, 0.7f, 0.7f };
	CreateMaterial(matDesc4, 4);


	MaterialDesc matDesc5;
	matDesc5.shaderType = E_SHADER_PRESET::DeferredSimple;
	matDesc5.textureCount_ = 2;
	matDesc5.ppTextures_ = new ITexture * [matDesc5.textureCount_];
	matDesc5.ppTextures_[0] = pWerewolf_Fur_Albedo;
	matDesc5.ppTextures_[1] = pWerewolf_Fur_Normal;
	matDesc5.shineness = 0.7f;
	matDesc5.specularColor = { 0.7f, 0.7f, 0.7f };
	CreateMaterial(matDesc5, 5);


	//MaterialDesc matDesc4;
	//matDesc4.shaderType = E_SHADER_PRESET::DeferredSimple;
	//TextureManager::Instance()->GetTexture(&matDesc4.pTextureAlbedo, 8);
	//TextureManager::Instance()->GetTexture(&matDesc4.pTextureNormal, 9);
	//matDesc4.shineness = 0.7f;
	//matDesc4.specularColor = { 0.7f, 0.7f, 0.7f };

	//matDesc4.textureCount_ = 3;
	//matDesc4.ppTextures_ = new ITexture * [matDesc4.textureCount_] {pTex1, pTex2, pTex3};
	//CreateMaterial(matDesc4, 4);
	//delete[] matDesc4.ppTextures_;

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
