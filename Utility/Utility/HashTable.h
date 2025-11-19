#pragma once
//struct BUCKET_DESC {
//	LINK_ITEM* bucketHead_;
//	LINK_ITEM* bucketTail_;
//	unsigned int linkCount_;
//};
//
//struct BUCKET_ITEM {
//	const void* item_;         // value
//	BUCKET_DESC* bucketDesc_;  // 버킷 아이템을 가지고 있는 버킷
//	LINK_ITEM link_;           // 버킷의 링크드리스트 링크
//	unsigned int keySize_;     // key size
//	char keyData_[1];
//};
//
//class HashTableIterator {
//	friend class HashTable;
//
//public:
//	UTILITY_API HashTableIterator(unsigned int maxBucketCount, unsigned int currentBucketCount, BUCKET_DESC* bucketDescTable, LINK_ITEM* current);
//
//	UTILITY_API HashTableIterator& operator++();  // ++전위
//
//	UTILITY_API HashTableIterator operator++(int);  // 후위++
//
//	UTILITY_API bool operator==(const HashTableIterator& other);
//
//	UTILITY_API bool operator!=(const HashTableIterator& other);
//
//	UTILITY_API void* operator*();
//
//private:
//	unsigned int maxBucketCount_;
//
//	unsigned int currentBucketCount_;
//
//	BUCKET_DESC* bucketDescTable_;
//
//	LINK_ITEM* current_;
//};
//
//// 중복키 허용
//class HashTable final {
//public:
//	UTILITY_API HashTable();
//
//	UTILITY_API ~HashTable();
//
//	UTILITY_API bool Initialize(unsigned int maxBucketCount, unsigned int maxKeySize);
//
//	UTILITY_API unsigned int Select(void** ppOutItemList, unsigned int maxItemCount, const void* keyData, unsigned int keySize, unsigned int startIndex = 0);
//
//	UTILITY_API void* Insert(const void* pItem, const void* pKeyData, unsigned int keySize);
//
//	UTILITY_API void Delete(const void* searchHandle);
//
//	UTILITY_API void DeleteAll();
//
//	UTILITY_API unsigned int GetAllItem(void** ppOutItemList, unsigned int maxItemCount, unsigned int startIndex, bool* pOutInsufficient) const;
//
//	UTILITY_API void Cleanup();
//
//	UTILITY_API HashTableIterator begin();
//
//	UTILITY_API HashTableIterator erase(HashTableIterator& iter);
//
//	UTILITY_API HashTableIterator end();
//
//private:
//	// 리턴은 해시테이블의 인덱스를 리턴한다.
//	unsigned int CreateKey(const void* pData, unsigned int keySize, unsigned int bucketCount);
//
//	BUCKET_DESC* bucketDescTable_;
//
//	unsigned int maxBucketCount_;
//
//	unsigned int maxKeySize_;
//
//	unsigned int currentItemCount_;
//};



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

	UTILITY_API void* Insert(void* pData, const void* pKeyData, unsigned int keySize);

	UTILITY_API bool Search(void** ppOutSearchedList, int* pOutSearchedCount, unsigned int countToSearch, const void* pKeyData, unsigned int keySize);

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

