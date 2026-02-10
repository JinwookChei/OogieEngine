#pragma once


struct HASH_BUCKET
{
	LinkedList chain_;
};

struct HASH_ENTRY
{
	unsigned int hash_;
	LINK_NODE linkNode_;
	void* pData_ = nullptr;
	unsigned int keySize_;
	char keyData[1];
};

class HashTableIterator
{
	friend class HashTable;

public:
	HashTableIterator() = delete;

	UTILITY_API HashTableIterator(HASH_BUCKET* pBucketTable, LINK_NODE* pCurLinkNode, unsigned int maxBucketCount, unsigned int curBucketCount);

	UTILITY_API HashTableIterator& operator++();

	UTILITY_API HashTableIterator operator++(int);

	UTILITY_API bool operator==(const HashTableIterator& other);

	UTILITY_API bool operator!=(const HashTableIterator& other);

	UTILITY_API void* operator*();

private:
	HASH_BUCKET* pBucketTable_;

	LINK_NODE* pCurLinkNode_;

	unsigned int maxBucketCount_;

	unsigned int curBucketCount_;
};


class HashTable
{
public:
	HashTable() = delete;

	UTILITY_API HashTable(unsigned int maxBucketCount, unsigned int maxKeySize);

	UTILITY_API ~HashTable();

	// 중복키 허용.
	UTILITY_API void* Insert(void* pData, const void* pKeyData, unsigned int keySize);

	UTILITY_API bool Search(void** ppOutSearchedList, unsigned int* pOutSearchedCount, unsigned int countToSearch, const void* pKeyData, unsigned int keySize);

	UTILITY_API void Delete(const void* pKeyData, unsigned int keySize);

	UTILITY_API void Delete(void* pHashHandle);

	UTILITY_API void Clear();

	UTILITY_API HashTableIterator begin();

	UTILITY_API HashTableIterator end();

	UTILITY_API HashTableIterator Delete(HashTableIterator& iter);

private:
	unsigned int CreateKey(const void* pKeyData, unsigned int keySize, unsigned int bucketCount);

	HASH_BUCKET* pBucketTable_;

	unsigned int maxBucketCount_;

	unsigned int maxKeySize_;

	unsigned int entryCount_;
};

