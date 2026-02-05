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
	const wchar_t* colorTexPath = L"../Resource/Texture/Bricks_4K/Bricks_Color.png";
	CreateTexture(colorTexPath, false, 1);

	const wchar_t* normalTexPath = L"../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
	CreateTexture(normalTexPath, true, 2);

	TextureDesc whiteTextureDesc;
	whiteTextureDesc.colorData_ = 0xFFFFFFFF;
	whiteTextureDesc.size_ = { 1.0f, 1.0f };
	ITexture* whiteTexture = Renderer::GetFactory()->CreateTexture(whiteTextureDesc);
	unsigned long long whiteTexTag = 3;
	textureTable_.Insert(whiteTexture, &whiteTexTag, 8);

	const wchar_t* maria_diffuse_Path = L"../Resource/Fbx/Mixamo/maria_diffuse.png";
	CreateTexture(maria_diffuse_Path, false, 4);
	const wchar_t* maria_normal_Path = L"../Resource/Fbx/Mixamo/maria_normal.png";
	CreateTexture(maria_normal_Path, true, 5);


	const wchar_t* wukong_torso_diffuse_Path = L"../Resource/Fbx/Wukong/Texture/Wukong_Torso_Diffuse.png";
	CreateTexture(wukong_torso_diffuse_Path, false, 6);
	const wchar_t* wukong_torso_normal_Path = L"../Resource/Fbx/Wukong/Texture/Wukong_Torso_Normal.png";
	CreateTexture(wukong_torso_normal_Path, true, 7);


	const wchar_t* wereWolf_Body_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Albedo.png";
	CreateTexture(wereWolf_Body_Albedo_Path, false, 8);
	const wchar_t* wereWolf_Body_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Normal.png";
	CreateTexture(wereWolf_Body_Normal_Path, true, 9);

	const wchar_t* wereWolf_Fur_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Albedo.png";
	CreateTexture(wereWolf_Fur_Albedo_Path, false, 10);
	const wchar_t* wereWolf_Fur_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Normal.png";
	CreateTexture(wereWolf_Fur_Normal_Path, true, 11);
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
