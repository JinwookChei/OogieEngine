#include "stdafx.h"


void LinkToLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew) {
    if (!*ppHead) {
        *ppTail = *ppHead = pNew;
        (*ppHead)->prev_ = nullptr;
        (*ppHead)->next_ = nullptr;
        return;
    }

    if (*ppHead == pNew) {
#ifdef _DEBUG
        __debugbreak();
#endif  // _DEBUG
        return;
    }

    pNew->next_ = (*ppHead);
    (*ppHead)->prev_ = pNew;
    *ppHead = pNew;
    pNew->prev_ = nullptr;
}

void LinkToLinkedListFIFO(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pNew) {
    if (!*ppHead) {
        *ppTail = *ppHead = pNew;
        (*ppHead)->prev_ = nullptr;
        (*ppHead)->next_ = nullptr;
        return;
    }

    if (*ppHead == pNew) {
#ifdef _DEBUG
        __debugbreak();
#endif  // _DEBUG
        return;
    }

    pNew->prev_ = (*ppTail);
    (*ppTail)->next_ = pNew;
    (*ppTail) = pNew;
    pNew->next_ = nullptr;
}

void UnLinkFromLinkedList(LINK_ITEM** ppHead, LINK_ITEM** ppTail, LINK_ITEM* pDel) {
#ifdef _DEBUG
    if (pDel->prev_ && pDel->prev_->next_ != pDel) {
        __debugbreak();
    }
#endif  // _DEBUG
    if (pDel->prev_) {
        pDel->prev_->next_ = pDel->next_;
    }
    else {
#ifdef _DEBUG
        if (pDel != (*ppHead)) {
            __debugbreak();
        }
#endif  // _DEBUG
        (*ppHead) = pDel->next_;
    }

    if (pDel->next_) {
        pDel->next_->prev_ = pDel->prev_;
    }
    else {
#ifdef _DEBUG
        if (pDel != (*ppTail)) {
            __debugbreak();
        }
#endif  // _DEBUG
        (*ppTail) = pDel->prev_;
    }

    pDel->next_ = nullptr;
    pDel->prev_ = nullptr;
}