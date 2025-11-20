#pragma once

//struct LINK_ITEM {
//	LINK_ITEM* prev_;
//	LINK_ITEM* next_;
//	void* item_;
//};
//
//UTILITY_API void LinkToLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew);
//UTILITY_API void LinkToLinkedListFIFO(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew);
//UTILITY_API void UnLinkFromLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pDel);



class LinkedList;
class HashTable;

struct LINK_NODE
{
	LINK_NODE* prev_ = nullptr;
	LINK_NODE* next_ = nullptr;
	void* pItem_ = nullptr;

private:
	friend class HashTable;
	friend class LinkedList;

	LinkedList* pOwner_ = nullptr;
};


class LinkedList
{
public:
	friend class HashTableIterator;
	friend class HashTable;
	UTILITY_API LinkedList();

	UTILITY_API ~LinkedList();

	UTILITY_API void PushFront(LINK_NODE* pNode);

	UTILITY_API void PushBack(LINK_NODE* pNode);

	UTILITY_API void Remove(LINK_NODE* pNode);

	UTILITY_API void Clean();
	
	UTILITY_API LINK_NODE* GetHead() const;

	UTILITY_API LINK_NODE* GetTail() const;

private:
	unsigned int count_;
	
	LINK_NODE* pHead_;

	LINK_NODE* pTail_;
};


