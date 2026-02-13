#pragma once


class MaterialManager
{
	friend class Engine;

	MaterialManager();

	~MaterialManager();
public:
	static ENGINE_API MaterialManager* Instance();

	void TestLoad();

	IMaterial* CreateMaterial(const MaterialDesc& desc, unsigned long long materialTag);

	ENGINE_API bool GetMaterial(IMaterial** ppOutMaterial, unsigned long long materialTag);

private:
	void CleanUp();

	HashTable materialTable_;
};