#include "stdafx.h"
#include "SkeletonManager.h"


SkeletonManager::SkeletonManager()
	: skeletonTable_(32, 32)
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

Skeleton* SkeletonManager::CreateSkeleton(const std::vector<Bone>& bones, const char* skeletonKey, unsigned int keySize)
{
	Skeleton* pNewSkeleton = new Skeleton;
	pNewSkeleton->bones_ = bones;
	skeletonTable_.Insert(pNewSkeleton, skeletonKey, keySize);
	return pNewSkeleton;
}

bool SkeletonManager::GetSkeleton(Skeleton** ppOutSkeleton, const char* skeletonKey, unsigned int keySize)
{
	unsigned int searchedCount = 0;
	void* pTmpMesh = nullptr;
	if (false == skeletonTable_.Search((void**)&pTmpMesh, &searchedCount, 8, skeletonKey, keySize))
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