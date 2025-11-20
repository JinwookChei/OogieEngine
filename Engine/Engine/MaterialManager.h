#pragma once


class MaterialManager
{
	friend class Engine;

	MaterialManager();

	~MaterialManager();
public:
	static MaterialManager* Instance();

	void TestLoad();

	IMaterial* CreateMaterial(const MaterialDesc& desc, unsigned long long materialTag);

	bool GetMaterial(IMaterial** ppOutMaterial, unsigned long long materialTag);

private:
	void CleanUp();

	HashTable materialTable_;
};