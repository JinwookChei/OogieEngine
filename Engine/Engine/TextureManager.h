#pragma once

class TextureManager
{
	friend class Engine;

	TextureManager();

	~TextureManager();

public:
	static TextureManager* Instance();

	void TestLoad();

	ITexture* CreateTextureFromFile(const wchar_t* fileName, bool isNormalMap, const char* textureKey, unsigned int keySize);	

	ITexture* CreateTexture(const TextureDesc& desc, const char* textureKey, unsigned int keySize);

	bool GetTexture(ITexture** ppOutTexture, const char* textureKey, unsigned int keySize);

private:
	void CleanUp();

	HashTable textureTable_;
};
