#include "stdafx.h"
#include "LinkedList.h"
#include "HashTable.h"

HashTableIterator::HashTableIterator(HASH_BUCKET* pBucketTable, LINK_NODE* pCurLinkNode, unsigned int maxBucketCount, unsigned int curBucketCount)
	: pBucketTable_(pBucketTable)
	, pCurLinkNode_(pCurLinkNode)
	, maxBucketCount_(maxBucketCount)
	, curBucketCount_(curBucketCount)
{
}

HashTableIterator& HashTableIterator::operator++()
{
	if (nullptr != pCurLinkNode_)
	{
		pCurLinkNode_ = pCurLinkNode_->next_;
	}
	if (nullptr != pCurLinkNode_)
	{
		return *this;
	}

	// 현재 버킷의 루프를 다돌았음
	// 다음 버킷의 루프를 돌아야함
	do {
		++curBucketCount_;
		if (maxBucketCount_ <= curBucketCount_ || nullptr == pBucketTable_)
		{
			curBucketCount_ = maxBucketCount_;
			pCurLinkNode_ = nullptr;
			pBucketTable_ = nullptr;
			break;
		}

		pCurLinkNode_ = pBucketTable_[curBucketCount_].chain_.pHead_;
	} while (nullptr == pCurLinkNode_);

	return *this;
}

HashTableIterator HashTableIterator::operator++(int)
{
	LINK_NODE* pTmpCurrent = pCurLinkNode_;

	if (nullptr != pTmpCurrent)
	{
		pTmpCurrent = pTmpCurrent->next_;
	}
	if (nullptr != pTmpCurrent)
	{
		return HashTableIterator(pBucketTable_, pTmpCurrent, maxBucketCount_, curBucketCount_);
	}

	// 현재 버킷의 루프를 다돌았음
	// 다음 버킷의 루프를 돌아야함
	unsigned int tmpCurrentBucketCount = curBucketCount_;
	do {
		++tmpCurrentBucketCount;
		if (maxBucketCount_ <= tmpCurrentBucketCount || nullptr == pBucketTable_)
		{
			return HashTableIterator(nullptr, nullptr, maxBucketCount_, maxBucketCount_);
		}

		pTmpCurrent = pBucketTable_[tmpCurrentBucketCount].chain_.pHead_;

	} while (nullptr == pTmpCurrent);

	return HashTableIterator(pBucketTable_, pTmpCurrent, maxBucketCount_, tmpCurrentBucketCount);
}

bool HashTableIterator::operator==(const HashTableIterator& other)
{
	return other.curBucketCount_ == curBucketCount_ && pCurLinkNode_ == other.pCurLinkNode_;
}

bool HashTableIterator::operator!=(const HashTableIterator& other)
{
	return false == (*this == other);
}

void* HashTableIterator::operator*()
{
	if (nullptr == pCurLinkNode_ || nullptr == pCurLinkNode_->pItem_)
	{
		return nullptr;
	}

	HASH_ENTRY* pHashEntry = (HASH_ENTRY*)pCurLinkNode_->pItem_;
	if (nullptr == pHashEntry->pData_)
	{
		__debugbreak();
	}
	return (void*)pHashEntry->pData_;
}


HashTable::HashTable(unsigned int maxBucketCount, unsigned int maxKeySize)
{
	maxBucketCount_ = maxBucketCount;
	maxKeySize_ = maxKeySize;
	entryCount_ = 0;

	pBucketTable_ = new HASH_BUCKET[maxBucketCount_];
}

HashTable::~HashTable()
{
	Clear();
	delete[] pBucketTable_;
}

void* HashTable::Insert(void* pData, const void* pKeyData, unsigned int keySize)
{
	if (nullptr == pData || nullptr == pKeyData || 0 == keySize || keySize > maxKeySize_)
	{
		__debugbreak();
		return nullptr;
	}

	unsigned int entryMemorySize = (unsigned int)(sizeof(HASH_ENTRY) - sizeof(char)) + maxKeySize_;
	HASH_ENTRY* pNewEntry = (HASH_ENTRY*)malloc(entryMemorySize);
	if (nullptr == pNewEntry)
	{
		__debugbreak();
		return nullptr;
	}

	unsigned int hash = CreateKey(pKeyData, keySize, maxBucketCount_);
	pNewEntry->hash_ = hash;
	pNewEntry->linkNode_.pItem_ = pNewEntry;
	pNewEntry->linkNode_.prev_ = nullptr;
	pNewEntry->linkNode_.next_ = nullptr;
	pNewEntry->linkNode_.pOwner_ = nullptr;
	pNewEntry->pData_ = pData;
	pNewEntry->keySize_ = keySize;
	memcpy(pNewEntry->keyData, pKeyData, keySize);

	pBucketTable_[hash].chain_.PushBack(&pNewEntry->linkNode_);
	++entryCount_;

	return pNewEntry;
}

bool HashTable::Search(void** ppOutSearchedList, unsigned int* pOutSearchedCount, unsigned int countToSearch, const void* pKeyData, unsigned int keySize)
{
	if (0 == keySize || 0 == countToSearch || nullptr == pKeyData)
	{
		__debugbreak();
		return false;
	}

	unsigned int searchedCount = 0;

	unsigned int hash = CreateKey(pKeyData, keySize, maxBucketCount_);
	LinkedList* pBucketChain = &pBucketTable_[hash].chain_;

	LINK_NODE* pCurNode = pBucketChain->pHead_;
	while (pCurNode)
	{
		HASH_ENTRY* pEntry = (HASH_ENTRY*)pCurNode->pItem_;
		pCurNode = pCurNode->next_;

		if (pEntry->keySize_ != keySize)
		{
			continue;
		}
		if (0 != memcmp(pEntry->keyData, pKeyData, keySize))
		{
			continue;
		}

		ppOutSearchedList[searchedCount] = pEntry->pData_;
		++searchedCount;

		if (searchedCount >= countToSearch)
		{
			break;
		}
	}

	*pOutSearchedCount = searchedCount;
	return *pOutSearchedCount > 0;
}

void HashTable::Delete(const void* pKeyData, unsigned int keySize)
{
	if (nullptr == pKeyData || 0 == keySize || maxKeySize_ < keySize)
	{
		__debugbreak();
		return;
	}

	unsigned int hash = CreateKey(pKeyData, keySize, maxBucketCount_);
	LinkedList* pBucketChain = &pBucketTable_[hash].chain_;

	LINK_NODE* pCurNode = pBucketChain->pHead_;
	while (pCurNode)
	{
		LINK_NODE pTmpNode = *pCurNode;
		HASH_ENTRY* pEntry = (HASH_ENTRY*)pCurNode->pItem_;

		if (pEntry->keySize_ != keySize)
		{
			pCurNode = pCurNode->next_;
			continue;
		}
		if (0 != memcmp(pEntry->keyData, pKeyData, keySize))
		{
			pCurNode = pCurNode->next_;
			continue;
		}

		pBucketChain->Remove(pCurNode);
		pCurNode = pTmpNode.next_;

		free(pEntry);
		--entryCount_;
	}
}

void HashTable::Delete(void* pHashHandle)
{
	if (nullptr == pHashHandle)
	{
		__debugbreak();
		return;
	}

	HASH_ENTRY* pEntry = static_cast<HASH_ENTRY*>(pHashHandle);
	unsigned int hash = pEntry->hash_;
	this->pBucketTable_[hash].chain_.Remove(&pEntry->linkNode_);


	free(pEntry);
	pEntry = nullptr;
	pHashHandle = nullptr;
	--entryCount_;
}

void HashTable::Clear()
{
	if (nullptr == pBucketTable_)
	{
		__debugbreak();
		return;
	}

	for (unsigned int n = 0; n < maxBucketCount_; ++n) {
		while (pBucketTable_[n].chain_.pHead_) {
			HASH_ENTRY* pEntry = (HASH_ENTRY*)pBucketTable_[n].chain_.pHead_->pItem_;
			Delete(pEntry);
		}
	}
}

HashTableIterator HashTable::begin()
{
	LINK_NODE* pCurLinkNode = nullptr;
	HASH_BUCKET* pBucketTable = pBucketTable_;

	unsigned int curBucketCount = -1;
	do {
		++curBucketCount;
		if (maxBucketCount_ <= curBucketCount || nullptr == pBucketTable) {
			curBucketCount = maxBucketCount_;
			pCurLinkNode = nullptr;
			pBucketTable = nullptr;
			break;
		}

		pCurLinkNode = pBucketTable[curBucketCount].chain_.pHead_;

	} while (nullptr == pCurLinkNode);

	return HashTableIterator(pBucketTable, pCurLinkNode, maxBucketCount_, curBucketCount);
}

HashTableIterator HashTable::end()
{
	return HashTableIterator(nullptr, nullptr, maxBucketCount_, maxBucketCount_);
}

HashTableIterator HashTable::Delete(HashTableIterator& iter)
{
	auto next = iter;
	++next;

	void* searchHandle = nullptr;
	if (nullptr != iter.pCurLinkNode_ && nullptr != iter.pCurLinkNode_->pItem_)
	{
		searchHandle = iter.pCurLinkNode_->pItem_;
	}

	if (searchHandle)
	{
		Delete(searchHandle);
	}

	return next;
}

unsigned int HashTable::CreateKey(const void* pKeyData, unsigned int keySize, unsigned int bucketCount)
{
	unsigned int keyData = 0;

	//  3
	// 0b00000011 0b00000001
	//  7
	// 0b00000111
	//  6
	// 0b00000110
	//  4
	// 0b00000100
	//  1
	// 0b00000001

	const char* pEntry = (const char*)pKeyData;
	if (keySize & 0x00000001) {
		keyData += (unsigned int)(*(unsigned char*)pEntry);
		++pEntry;
		--keySize;
	}
	// 6
	if (keySize & 0x00000002) {
		keyData += (unsigned int)(*(unsigned short*)pEntry);
		pEntry += 2;
		keySize -= 2;
	}

	keySize = keySize >> 2;

	for (unsigned int n = 0; n < keySize; ++n) {
		keyData += *(unsigned int*)pEntry;
		pEntry += 4;
	}

	return keyData % bucketCount;
}