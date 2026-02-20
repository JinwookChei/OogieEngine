#include "stdafx.h"
#include "SkeletonManager.h"


SkeletonManager::SkeletonManager()
	: skeletonTable_(16, 8)
{
}

SkeletonManager::~SkeletonManager()
{
	CleanUp();
}


SkeletonManager* SkeletonManager::Instance()
{
	return GSkeletonManager;
}

void SkeletonManager::TestLoad()
{

}

Skeleton* SkeletonManager::CreateSkeleton(const std::vector<Bone>& bones, unsigned long long skeletonTag)
{
	Skeleton* pNewSkeleton = new Skeleton;
	pNewSkeleton->bones_ = bones;
	skeletonTable_.Insert(pNewSkeleton, &skeletonTag, 8);
	return pNewSkeleton;
}

bool SkeletonManager::GetSkeleton(Skeleton** ppOutSkeleton, unsigned long long skeletonTag)
{
	unsigned int searchedCount = 0;
	void* pTmpMesh = nullptr;

	if (false == skeletonTable_.Search((void**)&pTmpMesh, &searchedCount, 8, &skeletonTag, 8))
	{
		DEBUG_BREAK();
		return false;
	}

	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}

	*ppOutSkeleton = static_cast<Skeleton*>(pTmpMesh);
	return true;
}

void SkeletonManager::CleanUp()
{
	HashTableIterator meshTableEnd = skeletonTable_.end();
	for (HashTableIterator iter = skeletonTable_.begin(); iter != meshTableEnd;)
	{
		Skeleton* pCurSkeleton = static_cast<Skeleton*>(*iter);
		if (nullptr != pCurSkeleton)
		{
			pCurSkeleton->Release();
			pCurSkeleton = nullptr;
		}
		iter = skeletonTable_.Delete(iter);
	}
}