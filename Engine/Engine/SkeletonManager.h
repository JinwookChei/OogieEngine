#pragma once

class SkeletonManager
{
	friend class Engine;
	SkeletonManager();
	~SkeletonManager();

public:
	static ENGINE_API SkeletonManager* Instance();

	void TestLoad();

	Skeleton* CreateSkeleton(const std::vector<Bone>& bones, const char* skeletonKey, unsigned int keySize);

	ENGINE_API bool GetSkeleton(Skeleton** ppOutSkeleton, const char* skeletonKey, unsigned int keySize);

private:
	void CleanUp();

	HashTable skeletonTable_;
};