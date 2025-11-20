#pragma once

class TextureManager
{
	friend class Engine;

	TextureManager();

	~TextureManager();

public:
	static TextureManager* Instance();

	void TestLoad();

	ITexture* CreateTexture(const wchar_t* fileName, bool isNormalMap, unsigned long long textureTag);

	bool GetTexture(ITexture** ppOutTexture, unsigned long long textureTag);

private:
	void CleanUp();

	HashTable textureTable_;
};
