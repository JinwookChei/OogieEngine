#pragma once
struct BUCKET_DESC {
	LINK_ITEM* bucketHead_;
	LINK_ITEM* bucketTail_;
	unsigned int linkCount_;
};

struct BUCKET_ITEM {
	const void* item_;         // value
	BUCKET_DESC* bucketDesc_;  // ��Ŷ �������� ������ �ִ� ��Ŷ
	LINK_ITEM link_;           // ��Ŷ�� ��ũ�帮��Ʈ ��ũ
	unsigned int keySize_;     // key size
	char keyData_[1];
};

class HashTableIterator {
	friend class HashTable;

public:
	UTILITY_API HashTableIterator(unsigned int maxBucketCount, unsigned int currentBucketCount, BUCKET_DESC* bucketDescTable, LINK_ITEM* current);

	UTILITY_API HashTableIterator& operator++();  // ++����

	UTILITY_API HashTableIterator operator++(int);  // ����++

	UTILITY_API bool operator==(const HashTableIterator& other);

	UTILITY_API bool operator!=(const HashTableIterator& other);

	UTILITY_API void* operator*();

private:
	unsigned int maxBucketCount_;

	unsigned int currentBucketCount_;

	BUCKET_DESC* bucketDescTable_;

	LINK_ITEM* current_;
};

// �ߺ�Ű ���
class HashTable final {
public:
	UTILITY_API HashTable();

	UTILITY_API ~HashTable();

	UTILITY_API bool Initialize(unsigned int maxBucketCount, unsigned int maxKeySize);

	UTILITY_API unsigned int Select(void** ppOutItemList, unsigned int maxItemCount, const void* keyData, unsigned int keySize, unsigned int startIndex = 0);

	UTILITY_API void* Insert(const void* pItem, const void* pKeyData, unsigned int keySize);

	UTILITY_API void Delete(const void* searchHandle);

	UTILITY_API void DeleteAll();

	UTILITY_API unsigned int GetAllItem(void** ppOutItemList, unsigned int maxItemCount, unsigned int startIndex, bool* pOutInsufficient) const;

	UTILITY_API void Cleanup();

	UTILITY_API HashTableIterator begin();

	UTILITY_API HashTableIterator erase(HashTableIterator& iter);

	UTILITY_API HashTableIterator end();

private:
	// ������ �ؽ����̺��� �ε����� �����Ѵ�.
	unsigned int CreateKey(const void* pData, unsigned int keySize, unsigned int bucketCount);

	BUCKET_DESC* bucketDescTable_;

	unsigned int maxBucketCount_;

	unsigned int maxKeySize_;

	unsigned int currentItemCount_;
};