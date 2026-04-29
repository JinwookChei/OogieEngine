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

	TextureDesc transparentTexDesc;
	transparentTexDesc.colorData_ = 0x00000000;
	transparentTexDesc.size_ = { 1.0f, 1.0f };
	ITexture* pTransparentTexture = Renderer::GetFactory()->CreateTexture(transparentTexDesc);
	unsigned long long transparentTexTag = 2;
	textureTable_.Insert(pTransparentTexture, &transparentTexTag, 8);

	
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
	const wchar_t* TerrainColorPath = L"../Resource/Texture/Terrain/4K/Terrain_Color_4k.png";
	CreateTexture(TerrainColorPath, false, 20);
	const wchar_t* TerrainNormalPath = L"../Resource/Texture/Terrain/4K/Terrain_Normal_4k.png";
	CreateTexture(TerrainNormalPath, true, 21);

	// House Texture
	const wchar_t* HouseDecalsAlbedoPath = L"../Resource/Fbx/House/00_Decals_Albedo.png";
	CreateTexture(HouseDecalsAlbedoPath, false, 100);
	const wchar_t* HouseDecalsNormalPath = L"../Resource/Fbx/House/00_Decals_Normal.png";
	CreateTexture(HouseDecalsNormalPath, true, 101);
	const wchar_t* HouseWoodTrimsAlbedoPath = L"../Resource/Fbx/House/01_Tileable_Wood_Trims_Albedo.png";
	CreateTexture(HouseWoodTrimsAlbedoPath, false, 102);
	const wchar_t* HouseWoodTrimsNormalPath = L"../Resource/Fbx/House/01_Tileable_Wood_Trims_Normal.png";
	CreateTexture(HouseWoodTrimsNormalPath, true, 103);
	const wchar_t* HouseRoofTileAlbedoPath = L"../Resource/Fbx/House/02_Roof_Tile_Albedo.png";
	CreateTexture(HouseRoofTileAlbedoPath, false, 104);
	const wchar_t* HouseRoofTileNormalPath = L"../Resource/Fbx/House/02_Roof_Tile_Normal.png";
	CreateTexture(HouseRoofTileNormalPath, true, 105);
	const wchar_t* HouseWoodEndAlbedoPath = L"../Resource/Fbx/House/03_Wood_End_Albedo.png";
	CreateTexture(HouseWoodEndAlbedoPath, false, 106);
	const wchar_t* HouseWoodEndNormalPath = L"../Resource/Fbx/House/03_Wood_End_Normal.png";
	CreateTexture(HouseWoodEndNormalPath, true, 107);
	const wchar_t* HouseWallPlasterAlbedoPath = L"../Resource/Fbx/House/04_Tileable_Wall_Plaster_Albedo.png";
	CreateTexture(HouseWallPlasterAlbedoPath, false, 108);
	const wchar_t* HouseWallPlasterNormalPath = L"../Resource/Fbx/House/04_Tileable_Wall_Plaster_Normal.png";
	CreateTexture(HouseWallPlasterNormalPath, true, 109);
	const wchar_t* HouseMetalAlbedoPath = L"../Resource/Fbx/House/05_Tileable_Metal_Albedo.png";
	CreateTexture(HouseMetalAlbedoPath, false, 110);
	const wchar_t* HouseMetalNormalPath = L"../Resource/Fbx/House/05_Tileable_Metal_Normal.png";
	CreateTexture(HouseMetalNormalPath, true, 111);
	const wchar_t* HouseWallStoneAlbedoPath = L"../Resource/Fbx/House/06_Tileable_Wall_Stone_Albedo.png";
	CreateTexture(HouseWallStoneAlbedoPath, false, 112);
	const wchar_t* HouseWallStoneNormalPath = L"../Resource/Fbx/House/06_Tileable_Wall_Stone_Normal.png";
	CreateTexture(HouseWallStoneNormalPath, true, 113);
	const wchar_t* HouseGlassWindowAlbedoPath = L"../Resource/Fbx/House/07_Tileable_Glass_Window_Albedo.png";
	CreateTexture(HouseGlassWindowAlbedoPath, false, 114);
	const wchar_t* HouseGlassWindowNormalPath = L"../Resource/Fbx/House/07_Tileable_Glass_Window_Normal.png";
	CreateTexture(HouseGlassWindowNormalPath, true, 115);

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
