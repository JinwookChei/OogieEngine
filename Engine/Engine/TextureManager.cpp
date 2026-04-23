#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
	: textureTable_(16, 8)
{
}

TextureManager::~TextureManager()
{
	CleanUp();
}

TextureManager* TextureManager::Instance()
{
	return GTextureManager;
}

void TextureManager::TestLoad()
{
	TextureDesc whiteTexDesc;
	whiteTexDesc.colorData_ = 0xFFFFFFFF;
	whiteTexDesc.size_ = { 1.0f, 1.0f };
	ITexture* pWhiteTexture = Renderer::GetFactory()->CreateTexture(whiteTexDesc);
	unsigned long long whiteTexTag = 0;
	textureTable_.Insert(pWhiteTexture, &whiteTexTag, 8);

	TextureDesc defaultNormalTexDesc;
	defaultNormalTexDesc.colorData_ = 0x8080FFFF;
	defaultNormalTexDesc.size_ = { 1.0f, 1.0f };
	ITexture* pdefaultNormalTexture = Renderer::GetFactory()->CreateTexture(defaultNormalTexDesc);
	unsigned long long defaultNormalTexTag = 1;
	textureTable_.Insert(pdefaultNormalTexture, &defaultNormalTexTag, 8);

	
	const wchar_t* colorTexPath = L"../Resource/Texture/Bricks_4K/Bricks_Color.png";
	CreateTexture(colorTexPath, false, 10);
	const wchar_t* normalTexPath = L"../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
	CreateTexture(normalTexPath, true, 11);
	const wchar_t* maria_diffuse_Path = L"../Resource/Fbx/Mixamo/maria_diffuse.png";
	CreateTexture(maria_diffuse_Path, false, 12);
	const wchar_t* maria_normal_Path = L"../Resource/Fbx/Mixamo/maria_normal.png";
	CreateTexture(maria_normal_Path, true, 13);
	const wchar_t* lightBulb_diffuse_path = L"../Resource/Fbx/LightBulb/T_Communication_BaseColor.png";
	CreateTexture(lightBulb_diffuse_path, false, 14);
	const wchar_t* lightBulb_normal_path = L"../Resource/Fbx/LightBulb/T_Communication_Normal.png";
	CreateTexture(lightBulb_normal_path, true, 15);
	const wchar_t* wereWolf_Body_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Albedo.png";
	CreateTexture(wereWolf_Body_Albedo_Path, false, 16);
	const wchar_t* wereWolf_Body_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Normal.png";
	CreateTexture(wereWolf_Body_Normal_Path, true, 17);
	const wchar_t* wereWolf_Fur_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Albedo.png";
	CreateTexture(wereWolf_Fur_Albedo_Path, false, 18);
	const wchar_t* wereWolf_Fur_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Normal.png";
	CreateTexture(wereWolf_Fur_Normal_Path, true, 19);
	const wchar_t* grassGroundColorPath = L"../Resource/Texture/GrassGround/4K/GrassGround_Color_4K.jpg";
	CreateTexture(grassGroundColorPath, false, 20);
	const wchar_t* grassGroundNormalPath = L"../Resource/Texture/GrassGround/4K/GrassGround_Normal_4K.png";
	CreateTexture(grassGroundNormalPath, true, 21);
}

ITexture* TextureManager::CreateTexture(const wchar_t* fileName, bool isNormalMap, unsigned long long textureTag)
{
	ITexture* pNewTexture = Renderer::GetFactory()->CreateTextureFromFile(fileName, isNormalMap);
	textureTable_.Insert(pNewTexture, &textureTag, 8);

	return pNewTexture;
}

bool TextureManager::GetTexture(ITexture** ppOutTexture, unsigned long long textureTag)
{
	unsigned int searchedCount = 0;
	void* pTmpTexture = nullptr;

	if (false == textureTable_.Search((void**)&pTmpTexture, &searchedCount, 8, &textureTag, 8))
	{
		Assert("Texture Search is Fail!!");
		return false;
	}

	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}

	*ppOutTexture = static_cast<ITexture*>(pTmpTexture);
	return true;
}

void TextureManager::CleanUp()
{
	HashTableIterator textureTableEnd = textureTable_.end();
	for (HashTableIterator iter = textureTable_.begin(); iter != textureTableEnd;)
	{
		ITexture* pCurTexture = static_cast<ITexture*>(*iter);
		if (nullptr != pCurTexture)
		{
			pCurTexture->Release();
			pCurTexture = nullptr;
		}
		iter = textureTable_.Delete(iter);
	}
}
