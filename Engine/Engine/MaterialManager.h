#pragma once


class MaterialManager
{
	friend class Engine;

	MaterialManager();

	~MaterialManager();
public:
	static ENGINE_API MaterialManager* Instance();

	void TestLoad();

	IMaterial* CreateMaterial(const MaterialDesc& desc, const char* materialKey, unsigned int keySize);

	ENGINE_API bool GetMaterial(IMaterial** ppOutMaterial, const char* materialKey, unsigned int keySize);

private:
	void CleanUp();

	HashTable materialTable_;
};