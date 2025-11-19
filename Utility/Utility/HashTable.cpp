#include "stdafx.h"
#include "LinkedList.h"
#include "HashTable.h"

//HashTableIterator::HashTableIterator(unsigned int maxBucketCount, unsigned int currentBucketCount, BUCKET_DESC* bucketDescTable, LINK_ITEM* current)
//	: maxBucketCount_(maxBucketCount),
//	currentBucketCount_(currentBucketCount),
//	bucketDescTable_(bucketDescTable),
//	current_(current) {
//}
//
//HashTableIterator& HashTableIterator::operator++() {
//	if (nullptr != current_) {
//		current_ = current_->next_;
//	}
//	if (nullptr != current_) {
//		return *this;
//	}
//
//	do {
//		++currentBucketCount_;
//		if (maxBucketCount_ <= currentBucketCount_ || nullptr == bucketDescTable_) {
//			currentBucketCount_ = maxBucketCount_;
//			current_ = nullptr;
//			bucketDescTable_ = nullptr;
//			break;
//		}
//
//		current_ = bucketDescTable_[currentBucketCount_].bucketHead_;
//
//	} while (nullptr == current_);
//
//	return *this;
//}
//
//HashTableIterator HashTableIterator::operator++(int) {
//	LINK_ITEM* tmpCurrent = current_;
//
//	if (nullptr != tmpCurrent) {
//		tmpCurrent = tmpCurrent->next_;
//	}
//	if (nullptr != tmpCurrent) {
//		return HashTableIterator(maxBucketCount_, currentBucketCount_, bucketDescTable_, tmpCurrent);
//	}
//
//	// 현재 버킷의 루프를 다돌았음
//	// 다음 버킷의 루프를 돌아야함
//	unsigned int tmpCurrentBucketCount = currentBucketCount_;
//	do {
//		++tmpCurrentBucketCount;
//		if (maxBucketCount_ <= tmpCurrentBucketCount || nullptr == bucketDescTable_) {
//			return HashTableIterator(maxBucketCount_, maxBucketCount_, nullptr, nullptr);
//		}
//
//		tmpCurrent = bucketDescTable_[tmpCurrentBucketCount].bucketHead_;
//
//	} while (nullptr == tmpCurrent);
//
//	return HashTableIterator(maxBucketCount_, tmpCurrentBucketCount, bucketDescTable_, tmpCurrent);
//}
//
//bool HashTableIterator::operator==(const HashTableIterator& other) {
//	return other.currentBucketCount_ == currentBucketCount_ && current_ == other.current_;
//}
//
//bool HashTableIterator::operator!=(const HashTableIterator& other) {
//	return false == (*this == other);
//}
//
//void* HashTableIterator::operator*() {
//	if (nullptr == current_ || nullptr == current_->item_) {
//		return nullptr;
//	}
//
//	BUCKET_ITEM* pBucket = (BUCKET_ITEM*)current_->item_;
//
//	return (void*)pBucket->item_;
//}
//
//HashTable::HashTable()
//	: bucketDescTable_(nullptr),
//	maxBucketCount_(0),
//	maxKeySize_(0),
//	currentItemCount_(0) {
//}
//
//HashTable::~HashTable() {
//	Cleanup();
//}
//
//bool HashTable::Initialize(unsigned int maxBucketCount, unsigned int maxKeySize) {
//	maxBucketCount_ = maxBucketCount;
//	maxKeySize_ = maxKeySize;
//
//	bucketDescTable_ = (BUCKET_DESC*)malloc(sizeof(BUCKET_DESC) * maxBucketCount_);
//	if (nullptr == bucketDescTable_) {
//		return false;
//	}
//
//	// 1바이트 마다 어떤값으로 채울꺼냐.
//	memset(bucketDescTable_, 0, sizeof(BUCKET_DESC) * maxBucketCount_);
//
//	return true;
//}
//
//unsigned int HashTable::Select(void** ppOutItemList, unsigned int maxItemCount, const void* keyData, unsigned int keySize, unsigned int startIndex /*= 0*/) {
//	if (0 == maxItemCount || nullptr == ppOutItemList || nullptr == keyData || 0 == keySize) {
//		return 0;
//	}
//	unsigned int seletedItemCount = 0;  // 몇개 찾았는지
//
//	unsigned int findLoopCount = 0;
//
//	// 몇개 찾아야하지? maxItemCount 만큼 찾아야한다.
//	// 찾으면? 어디다 담아줘야할까? ppOutItemList 에 담아줘야한다.
//	// 무엇을 찾아야할까? keyData 를 찾아야한다.
//	// keyData Size 가 0 이면? 못찾음
//
//	// 어떤 Bucket 에 keyData 값이 존재할까? 버킷을 어떻게 찾지?
//	// CreateKey 구현내용
//	// keyData 를 HashKey 만들어서 % (모드연산) 후에 인덱스를 찾는다. 그러면 모드연산은 누구랑 해야할까?
//	// HashKey % maxBucketCount_ 모드 연산하면 인덱스가 나온다. ( 0 ~ (maxBucketCount_-1) ) 값이 나온다.
//	unsigned int hashTableIndex = CreateKey(keyData, keySize, maxBucketCount_);
//	BUCKET_DESC* pBucketDesc = &bucketDescTable_[hashTableIndex];  // bucketDescTable_ + hashTableIndex;
//
//	LINK_ITEM* pCur = pBucketDesc->bucketHead_;
//	while (pCur) {
//		if (0 == maxItemCount) {
//			return seletedItemCount;
//		}
//
//		BUCKET_ITEM* pBucket = (BUCKET_ITEM*)pCur->item_;
//		pCur = pCur->next_;
//
//		if (pBucket->keySize_ != keySize) {
//			continue;
//		}
//		if (0 != memcmp(pBucket->keyData_, keyData, keySize)) {
//			continue;
//		}
//
//		if (startIndex <= findLoopCount) {
//			ppOutItemList[seletedItemCount] = (void*)pBucket->item_;
//			--maxItemCount;
//			++seletedItemCount;
//		}
//
//		++findLoopCount;
//	}
//
//	return seletedItemCount;
//}
//
//void* HashTable::Insert(const void* pItem, const void* pKeyData, unsigned int keySize) {
//	if (nullptr == pItem || nullptr == pKeyData || 0 == keySize || maxKeySize_ < keySize) {
//		__debugbreak();
//		return nullptr;
//	}
//
//	unsigned int bucketMemorySize = (unsigned int)(sizeof(BUCKET_ITEM) - sizeof(char)) + maxKeySize_;
//	BUCKET_ITEM* pBucket = (BUCKET_ITEM*)malloc(bucketMemorySize);
//	if (nullptr == pBucket) {
//		__debugbreak();
//		return nullptr;
//	}
//
//	// pItem 을 Bucket 에 저장한다. 저장할때 사용하는 키는 pKeyData
//	// 저장되어야하는 Bucket 을 찾아야함
//	// pKeyData 를 해쉬로 변경하고 변경된 해쉬를 maxBucketCount_ 값으로 모드 연산하면 Bucket 의 인덱스가 된다.
//	// 그러므로 Bucket 을 찾을 수 있다.
//
//	unsigned int hashTableIndex = CreateKey(pKeyData, keySize, maxBucketCount_);
//	BUCKET_DESC* pBucketDesc = &bucketDescTable_[hashTableIndex];
//
//	pBucket->item_ = pItem;
//	pBucket->keySize_ = keySize;
//	pBucket->bucketDesc_ = pBucketDesc;
//	pBucket->link_.next_ = nullptr;
//	pBucket->link_.prev_ = nullptr;
//	pBucket->link_.item_ = pBucket;
//	++pBucketDesc->linkCount_;
//
//	memcpy(pBucket->keyData_, pKeyData, keySize);
//
//	LinkToLinkedListFIFO(&pBucketDesc->bucketHead_, &pBucketDesc->bucketTail_, &pBucket->link_);
//
//	++currentItemCount_;
//
//	return pBucket;
//}
//
//void HashTable::Delete(const void* searchHandle) {
//	if (nullptr == searchHandle) {
//		return;
//	}
//
//	BUCKET_ITEM* pBucket = (BUCKET_ITEM*)searchHandle;
//	BUCKET_DESC* pBucketDesc = pBucket->bucketDesc_;
//
//	UnLinkFromLinkedList(&pBucketDesc->bucketHead_, &pBucketDesc->bucketTail_, &pBucket->link_);
//
//	--pBucketDesc->linkCount_;
//	--currentItemCount_;
//
//	free(pBucket);
//}
//
//void HashTable::DeleteAll() {
//	if (nullptr == bucketDescTable_) {
//		return;
//	}
//	for (unsigned int n = 0; n < maxBucketCount_; ++n) {
//		while (bucketDescTable_[n].bucketHead_) {
//			BUCKET_ITEM* pBucket = (BUCKET_ITEM*)bucketDescTable_[n].bucketHead_->item_;
//			Delete(pBucket);
//		}
//	}
//}
//
//unsigned int HashTable::GetAllItem(void** ppOutItemList, unsigned int maxItemCount, unsigned int startIndex, bool* pOutInsufficient) const {
//	// 순차적으로 버킷테이블을 돌면서 startIndex 부터 아이템들을 maxItemCount 만큼 ppOutItemList 채워서 리턴한다.
//	// 리턴을 할때 아이템이 더 존재하면 pOutInsufficient 값에 true 를 넣어줘서 밖에 전파를 한다.
//
//	if (nullptr != pOutInsufficient) {
//		*pOutInsufficient = false;
//	}
//
//	unsigned int itemCount = 0;
//
//	unsigned int allItemLoopCount = 0;
//
//	for (unsigned int n = 0; n < maxBucketCount_; ++n) {
//		LINK_ITEM* pCur = bucketDescTable_[n].bucketHead_;
//		while (pCur) {
//			BUCKET_ITEM* pBucket = (BUCKET_ITEM*)pCur->item_;
//			if (maxItemCount <= itemCount) {
//				if (nullptr != pOutInsufficient) {
//					*pOutInsufficient = true;
//				}
//				return itemCount;
//			}
//
//			if (startIndex <= allItemLoopCount) {
//				ppOutItemList[itemCount] = (void*)pBucket->item_;
//				++itemCount;
//			}
//
//			++allItemLoopCount;
//			pCur = pCur->next_;
//		}
//	}
//
//	return itemCount;
//}
//
//void HashTable::Cleanup() {
//	DeleteAll();
//	maxBucketCount_ = 0;
//	if (nullptr != bucketDescTable_) {
//		free(bucketDescTable_);
//		bucketDescTable_ = nullptr;
//	}
//}
//
//HashTableIterator HashTable::begin() {
//	LINK_ITEM* current = nullptr;
//	BUCKET_DESC* bucketDescTable = bucketDescTable_;
//
//	unsigned int currentBucketCount = -1;
//	do {
//		++currentBucketCount;
//		if (maxBucketCount_ <= currentBucketCount || nullptr == bucketDescTable) {
//			currentBucketCount = maxBucketCount_;
//			current = nullptr;
//			bucketDescTable = nullptr;
//			break;
//		}
//
//		current = bucketDescTable[currentBucketCount].bucketHead_;
//
//	} while (nullptr == current);
//
//	return HashTableIterator(maxBucketCount_, currentBucketCount, bucketDescTable, current);
//}
//
//HashTableIterator HashTable::erase(HashTableIterator& iter) {
//	void* searchHandle = nullptr;
//	if (nullptr != iter.current_ && nullptr != iter.current_->item_) {
//		searchHandle = iter.current_->item_;
//	}
//
//	++iter;
//
//	if (searchHandle) {
//		Delete(searchHandle);
//	}
//
//	return iter;
//}
//
//HashTableIterator HashTable::end() {
//	return HashTableIterator(maxBucketCount_, maxBucketCount_, nullptr, nullptr);
//}
//
//unsigned int HashTable::CreateKey(const void* pData, unsigned int keySize, unsigned int bucketCount) {
//	unsigned int keyData = 0;
//
//	//  3
//	// 0b00000011 0b00000001
//	//  7
//	// 0b00000111
//	//  6
//	// 0b00000110
//	//  4
//	// 0b00000100
//	//  1
//	// 0b00000001
//
//	const char* pEntry = (const char*)pData;
//	if (keySize & 0x00000001) {
//		keyData += (unsigned int)(*(unsigned char*)pEntry);
//		++pEntry;
//		--keySize;
//	}
//	// 6
//	if (keySize & 0x00000002) {
//		keyData += (unsigned int)(*(unsigned short*)pEntry);
//		pEntry += 2;
//		keySize -= 2;
//	}
//
//	keySize = keySize >> 2;
//
//	for (unsigned int n = 0; n < keySize; ++n) {
//		keyData += *(unsigned int*)pEntry;
//		pEntry += 4;
//	}
//
//	return keyData % bucketCount;
//}
//

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

bool HashTable::Search(void** ppOutSearchedList, int* pOutSearchedCount, unsigned int countToSearch, const void* pKeyData, unsigned int keySize)
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