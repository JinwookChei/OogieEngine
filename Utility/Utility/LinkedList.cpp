#include "stdafx.h"
#include "LinkedList.h"

LinkedList::LinkedList()
	: count_(0)
	, pHead_(nullptr)
	, pTail_(nullptr)
{
}

LinkedList::~LinkedList()
{
}

void LinkedList::PushFront(LINK_NODE* pNode)
{
		if (nullptr == pNode)
		{
			return;
		}

		if (nullptr != pNode->pOwner_)
		{
			__debugbreak();
			return;
		}

		if (nullptr == pHead_)
		{
			pHead_ = pTail_ = pNode;
		}
		else
		{
			pNode->next_ = pHead_;
			pHead_->prev_ = pNode;
			pHead_ = pNode;
			pHead_->prev_ = nullptr;
		}

		pNode->pOwner_ = this;
		++count_;
	
}

void LinkedList::PushBack(LINK_NODE* pNode)
{
	if (nullptr == pNode)
	{
		return;
	}

	if (nullptr != pNode->pOwner_)
	{
		__debugbreak();
		return;
	}

	if (nullptr == pTail_)
	{
		pHead_ = pTail_ = pNode;
	}
	else
	{
		pNode->prev_ = pTail_;
		pTail_->next_ = pNode;
		pTail_ = pNode;
		pTail_->next_ = nullptr;
	}

	pNode->pOwner_ = this;
	++count_;
}


void LinkedList::Remove(LINK_NODE* pNode)
{
	if (nullptr == pNode)
	{
		__debugbreak();
		return;
	}
	if (pNode->pOwner_ != this)
	{
		__debugbreak();
		return;
	}

	if (pHead_ == pNode && pTail_ == pNode)
	{
		pNode->next_ = nullptr;
		pNode->prev_ = nullptr;
		pHead_ = nullptr;
		pTail_ = nullptr;
	}
	else if (pNode == pHead_)
	{
		pHead_ = pNode->next_;
		pNode->next_ = nullptr;
		pHead_->prev_ = nullptr;
	}
	else if (pNode == pTail_)
	{
		pTail_ = pNode->prev_;
		pNode->prev_ = nullptr;
		pTail_->next_ = nullptr;
	}
	else
	{
		pNode->prev_->next_ = pNode->next_;
		pNode->next_->prev_ = pNode->prev_;
		pNode->prev_ = nullptr;
		pNode->next_ = nullptr;
	}

	pNode->pOwner_ = nullptr;
	--count_;
}

void LinkedList::Clean()
{
	while (pHead_)
	{
		Remove(pHead_);
	}
}

UTILITY_API LINK_NODE* LinkedList::GetHead() const
{
	return pHead_;
}

UTILITY_API LINK_NODE* LinkedList::GetTail() const
{
	return pTail_;
}
