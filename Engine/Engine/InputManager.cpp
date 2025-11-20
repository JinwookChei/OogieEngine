#include "stdafx.h"
#include "InputManager.h"


InputManager::InputState::InputState() 
{
}

InputManager::InputState::InputState(int key)
    : key_(key) 
{
}

void InputManager::InputState::InputStateCheck(unsigned long long deltaTick) 
{
    short ret = GetAsyncKeyState(key_);
    if (0 != ret) {
        pressTime_ += deltaTick;
        if (true == free_) {
            down_ = true;
            press_ = true;
            up_ = false;
            free_ = false;
        }
        else if (true == down_) {
            down_ = false;
            press_ = true;
            up_ = false;
            free_ = false;
        }
    }
    else {
        upTime_ += deltaTick;
        if (true == press_) {
            pressTime_ = 0;
            upTime_ = 0;
            down_ = false;
            press_ = false;
            up_ = true;
            free_ = false;
        }
        else if (true == up_) {
            down_ = false;
            press_ = false;
            up_ = false;
            free_ = true;
        }
    }
}

InputManager::InputManager()
    : isAnyKeyDown_(false), 
    isAnyKeyPress_(false), 
    hashTable_(nullptr) 
{
}

InputManager::~InputManager() 
{
    if (nullptr == hashTable_) 
    {
        return;
    }

    HashTableIterator endIter = hashTable_->end();
    for (HashTableIterator iter = hashTable_->begin(); iter != endIter;)
    {
        InputState* pCur = (InputState*)*iter;
        iter = hashTable_->Delete(iter);
        delete pCur;
    }
    
    delete hashTable_;
    hashTable_ = nullptr;
}

InputManager* InputManager::Instance()
{
    return GInputManager;
}

bool InputManager::IsAnyKeyDown() const
{
    return isAnyKeyDown_;
}

bool InputManager::IsAnyKeyPress() const
{
    return isAnyKeyPress_;
}

bool InputManager::IsDown(int key) 
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return false;
    }

    return pFind->down_;
}

bool InputManager::IsPress(int key) 
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return false;
    }

    return pFind->press_;
}

unsigned long long InputManager::PressTime(int key)
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return 0;
    }

    return pFind->pressTime_;
}

bool InputManager::IsUp(int key) 
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return false;
    }

    return pFind->up_;
}

bool InputManager::IsFree(int key)
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return false;
    }

    return pFind->free_;
}

unsigned long long InputManager::UpTime(int key)
{
    InputState* pFind = nullptr;
    unsigned int searchedCount;
    if (false == hashTable_->Search((void**)&pFind, &searchedCount, 1, &key, 4))
    {
        return 0;
    }

    return pFind->upTime_;
}

const Float2& InputManager::GetPrevMousePosition() const
{
    return prevMousePos_;
}

const Float2& InputManager::GetCurrentMousePosition() const
{
    return curMousePos_;
}

const Float2& InputManager::GetDeltaMouseMove() const
{
    return deltaMouseMove_;
}

bool InputManager::Initialize()
{
    hashTable_ = new HashTable(32, sizeof(int));

    InputState* newState = new InputState(VK_LBUTTON);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_RBUTTON);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_MBUTTON);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_SPACE);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_RETURN);
    hashTable_->Insert(newState, &newState->key_, 4);

    newState = new InputState(VK_LEFT);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_RIGHT);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_UP);
    hashTable_->Insert(newState, &newState->key_, 4);
    newState = new InputState(VK_DOWN);
    hashTable_->Insert(newState, &newState->key_, 4);

    for (int n = VK_F1; n < VK_F13; ++n) 
    {
        newState = new InputState(n);
        hashTable_->Insert(newState, &newState->key_, 4);
    }
    for (int n = 'A'; n <= 'Z'; ++n)
    {
        newState = new InputState(n);
        hashTable_->Insert(newState, &newState->key_, 4);
    }

    constexpr int interval = 'a' - 'A';
    for (int n = 'a'; n <= 'z'; ++n) 
    {
        int realKey = n - interval;
        newState = new InputState(realKey);
        hashTable_->Insert(newState, &n, 4);
    }
    for (int n = '0'; n <= '9'; ++n) 
    {
        newState = new InputState(n);
        hashTable_->Insert(newState, &newState->key_, 4);
    }

    return true;
}

void InputManager::Tick(double deltaTime)
{
    UpdateInputState(deltaTime);

    UpdateMouseMove();
}

void InputManager::UpdateInputState(double deltaTime)
{
    if (nullptr == hashTable_) {
        return;
    }

    isAnyKeyDown_ = false;
    isAnyKeyPress_ = false;

    HashTable& tmpTable = *hashTable_;

    for (void* value : tmpTable) {
        InputState* tmpValue = (InputState*)value;

        tmpValue->InputStateCheck(deltaTime);

        if (false == isAnyKeyDown_ && tmpValue->down_) {
            isAnyKeyDown_ = true;
        }
        if (false == isAnyKeyPress_ && tmpValue->press_) {
            isAnyKeyPress_ = true;
        }
    }
}

void InputManager::UpdateMouseMove()
{
    prevMousePos_ = curMousePos_;

    curMousePos_ = GApplication->GetMousePosition();

    deltaMouseMove_ = curMousePos_ - prevMousePos_;
}
