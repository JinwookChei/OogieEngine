#pragma once

class MeshManager
{
	friend class Engine;
	MeshManager();
	~MeshManager();

public:
	static MeshManager* Instance();

	void TestLoad();

	IMesh* CreateMesh(const MeshDesc& desc, unsigned long long meshTag);

	bool GetMesh(IMesh** ppOutMesh, unsigned long long meshTag);

private:
	void CleanUp();

	HashTable meshTable_;
};