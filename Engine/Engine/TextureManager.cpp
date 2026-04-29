#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
	: textureTable_(32, 32)
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
	CreateTexture(whiteTexDesc, "T_White", 7);

	TextureDesc defaultNormalTexDesc;
	defaultNormalTexDesc.colorData_ = 0x8080FFFF;
	defaultNormalTexDesc.size_ = { 1.0f, 1.0f };
	CreateTexture(defaultNormalTexDesc, "T_DefaultNormal", 15);
	
	TextureDesc transparentTexDesc;
	transparentTexDesc.colorData_ = 0x00000000;
	transparentTexDesc.size_ = { 1.0f, 1.0f };
	CreateTexture(transparentTexDesc, "T_Transparent", 13);

	const wchar_t* bricks_Albedo_Path = L"../Resource/Texture/Bricks_4K/Bricks_Color.png";
	CreateTextureFromFile(bricks_Albedo_Path, false, "T_Bricks_Albedo", 15);
	const wchar_t* bricks_Normal_Path = L"../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
	CreateTextureFromFile(bricks_Normal_Path, true, "T_Bricks_Normal", 15);
	const wchar_t* maria_Albedo_Path = L"../Resource/Fbx/Mixamo/maria_diffuse.png";
	CreateTextureFromFile(maria_Albedo_Path, false, "T_Maria_Albedo", 14);
	const wchar_t* maria_Normal_Path = L"../Resource/Fbx/Mixamo/maria_normal.png";
	CreateTextureFromFile(maria_Normal_Path, true,"T_Maria_Normal", 14);
	const wchar_t* lightBulb_Albedo_Path = L"../Resource/Fbx/LightBulb/T_Communication_BaseColor.png";
	CreateTextureFromFile(lightBulb_Albedo_Path, false, "T_LightBulb_Albedo", 18);
	const wchar_t* lightBulb_Normal_Path = L"../Resource/Fbx/LightBulb/T_Communication_Normal.png";
	CreateTextureFromFile(lightBulb_Normal_Path, true,"T_LightBulb_Normal", 18);
	const wchar_t* werewolf_Body_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Albedo.png";
	CreateTextureFromFile(werewolf_Body_Albedo_Path, false,"T_Werewolf_Body_Albedo", 22);
	const wchar_t* werewolf_Body_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Body_Normal.png";
	CreateTextureFromFile(werewolf_Body_Normal_Path, true, "T_Werewolf_Body_Normal", 22);
	const wchar_t* werewolf_Fur_Albedo_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Albedo.png";
	CreateTextureFromFile(werewolf_Fur_Albedo_Path, false, "T_Werewolf_Fur_Albedo", 21);
	const wchar_t* werewolf_Fur_Normal_Path = L"../Resource/Fbx/WereWolf/Texture/Werewolf_Fur_Normal.png";
	CreateTextureFromFile(werewolf_Fur_Normal_Path, true, "T_Werewolf_Fur_Normal", 21);
	const wchar_t* terrain_Albedo_Path = L"../Resource/Texture/Terrain/4K/Terrain_Color_4k.png";
	CreateTextureFromFile(terrain_Albedo_Path, false, "T_Terrain_Albedo", 16);
	const wchar_t* terrain_Normal_Path = L"../Resource/Texture/Terrain/4K/Terrain_Normal_4k.png";
	CreateTextureFromFile(terrain_Normal_Path, true, "T_Terrain_Normal", 16);

	// House Texture
	const wchar_t* house_Decals_Albedo_Path = L"../Resource/Fbx/House/00_Decals_Albedo.png";
	CreateTextureFromFile(house_Decals_Albedo_Path, false, "T_House_Decals_Albedo", 21);
	const wchar_t* house_Decals_Normal_Path = L"../Resource/Fbx/House/00_Decals_Normal.png";
	CreateTextureFromFile(house_Decals_Normal_Path, true, "T_House_Decals_Normal", 21);
	const wchar_t* house_WoodTrims_Albedo_Path = L"../Resource/Fbx/House/01_Tileable_Wood_Trims_Albedo.png";
	CreateTextureFromFile(house_WoodTrims_Albedo_Path, false, "T_House_WoodTrims_Albedo", 24);
	const wchar_t* house_WoodTrims_Normal_Path = L"../Resource/Fbx/House/01_Tileable_Wood_Trims_Normal.png";
	CreateTextureFromFile(house_WoodTrims_Normal_Path, true, "T_House_WoodTrims_Normal", 24);
	const wchar_t* house_RoofTile_Albedo_Path = L"../Resource/Fbx/House/02_Roof_Tile_Albedo.png";
	CreateTextureFromFile(house_RoofTile_Albedo_Path, false,"T_House_RoofTile_Albedo", 23);
	const wchar_t* house_RoofTile_Normal_Path = L"../Resource/Fbx/House/02_Roof_Tile_Normal.png";
	CreateTextureFromFile(house_RoofTile_Normal_Path, true, "T_House_RoofTile_Normal", 23);
	const wchar_t* house_WoodEnd_Albedo_Path = L"../Resource/Fbx/House/03_Wood_End_Albedo.png";
	CreateTextureFromFile(house_WoodEnd_Albedo_Path, false, "T_House_WoodEnd_Albedo", 22);
	const wchar_t* house_WoodEnd_Normal_Path = L"../Resource/Fbx/House/03_Wood_End_Normal.png";
	CreateTextureFromFile(house_WoodEnd_Normal_Path, true, "T_House_WoodEnd_Normal", 22);
	const wchar_t* house_WallPlaster_Albedo_Path = L"../Resource/Fbx/House/04_Tileable_Wall_Plaster_Albedo.png";
	CreateTextureFromFile(house_WallPlaster_Albedo_Path, false, "T_House_WallPlaster_Albedo", 26);
	const wchar_t* house_WallPlaster_Normal_Path = L"../Resource/Fbx/House/04_Tileable_Wall_Plaster_Normal.png";
	CreateTextureFromFile(house_WallPlaster_Normal_Path, true,"T_House_WallPlaster_Normal", 26);
	const wchar_t* house_Metal_Albedo_Path = L"../Resource/Fbx/House/05_Tileable_Metal_Albedo.png";
	CreateTextureFromFile(house_Metal_Albedo_Path, false, "T_House_Metal_Albedo", 20);
	const wchar_t* house_Metal_Normal_Path = L"../Resource/Fbx/House/05_Tileable_Metal_Normal.png";
	CreateTextureFromFile(house_Metal_Normal_Path, true, "T_House_Metal_Normal", 20);
	const wchar_t* house_WallStone_Albedo_Path = L"../Resource/Fbx/House/06_Tileable_Wall_Stone_Albedo.png";
	CreateTextureFromFile(house_WallStone_Albedo_Path, false, "T_House_WallStone_Albedo", 24);
	const wchar_t* house_WallStone_Normal_Path = L"../Resource/Fbx/House/06_Tileable_Wall_Stone_Normal.png";
	CreateTextureFromFile(house_WallStone_Normal_Path, true, "T_House_WallStone_Normal", 24);
	const wchar_t* house_GlassWindow_Albedo_Path = L"../Resource/Fbx/House/07_Tileable_Glass_Window_Albedo.png";
	CreateTextureFromFile(house_GlassWindow_Albedo_Path, false, "T_House_GlassWindow_Albedo", 26);
	const wchar_t* house_GlassWindow_Normal_Path = L"../Resource/Fbx/House/07_Tileable_Glass_Window_Normal.png";
	CreateTextureFromFile(house_GlassWindow_Normal_Path, true, "T_House_GlassWindow_Normal", 26);
}

ITexture* TextureManager::CreateTextureFromFile(const wchar_t* fileName, bool isNormalMap, const char* textureKey, unsigned int keySize)
{
	ITexture* pNewTexture = Renderer::GetFactory()->CreateTextureFromFile(fileName, isNormalMap);
	textureTable_.Insert(pNewTexture, textureKey, keySize);
	return pNewTexture;
}

ITexture* TextureManager::CreateTexture(const TextureDesc& desc, const char* textureKey, unsigned int keySize)
{
	ITexture* pWhiteTexture = Renderer::GetFactory()->CreateTexture(desc);
	textureTable_.Insert(pWhiteTexture, textureKey, keySize);
	return pWhiteTexture;
}

bool TextureManager::GetTexture(ITexture** ppOutTexture, const char* textureKey, unsigned int keySize)
{
	unsigned int searchedCount = 0;
	void* pTmpTexture = nullptr;
	if (false == textureTable_.Search((void**)&pTmpTexture, &searchedCount, 8, textureKey, keySize))
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
