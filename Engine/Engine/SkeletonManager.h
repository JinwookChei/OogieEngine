#pragma once

class SkeletonManager
{
	friend class Engine;
	SkeletonManager();
	~SkeletonManager();

public:
	static ENGINE_API SkeletonManager* Instance();

	void TestLoad();

	Skeleton* CreateSkeleton(const std::vector<Bone>& bones, unsigned long long skeletonTag);

	ENGINE_API bool GetSkeleton(Skeleton** ppOutSkeleton, unsigned long long skeletonTag);

private:
	void CleanUp();

	HashTable skeletonTable_;
};