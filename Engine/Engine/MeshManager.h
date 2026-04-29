#pragma once

class MeshManager
{
	friend class Engine;
	MeshManager();
	~MeshManager();

public:
	static ENGINE_API  MeshManager* Instance();

	void TestLoad();

	IMesh* CreateMesh(const MeshDesc& desc, const char* meshKey, unsigned int keySize);

	ENGINE_API bool GetMesh(IMesh** ppOutMesh, const char* meshKey, unsigned int keySize);

private:
	void CleanUp();

	HashTable meshTable_;
};