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
	//pTextureColor_ = GRenderer->LoadTextureFromDirectXTex(texPath, false);

	const wchar_t* normalTexPath = L"../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
	CreateTexture(normalTexPath, true, 2);

	//pTextureNormal_ = GRenderer->LoadTextureFromDirectXTex(texPath, true);
}

ITexture* TextureManager::CreateTexture(const wchar_t* fileName, bool isNormalMap, unsigned long long textureTag)
{
	ITexture* pNewTexture = GRenderer->LoadTextureFromDirectXTex(fileName, isNormalMap);
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
