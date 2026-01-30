#pragma once

class HashTable;
class InputManager final {
private:
    friend class Engine;

    class InputState 
    {
        friend class InputManager;

        InputState();
        InputState(int key);
        void InputStateCheck(unsigned long long deltaTick);

    private:
        bool down_ = false; 
        bool press_ = false;
        bool up_ = false;   
        bool free_ = true;  
        int key_ = -1;

        unsigned long long pressTime_ = 0;
        unsigned long long upTime_ = 0;
    };

private:
    friend class Engine;
    InputManager();
    ~InputManager();

    static InputManager* GInputManager;
    static bool Init();
    static void Tick(double deltaTime);
    static void CleanUp();

    bool KeySetting();
    void UpdateInputState(double deltaTime);
    void UpdateMouseMove();
public:

    ENGINE_API static bool IsAnyKeyDown();
    ENGINE_API static bool IsAnyKeyPress();
    ENGINE_API static bool IsDown(int key);
    ENGINE_API static bool IsPress(int key);
    ENGINE_API static unsigned long long PressTime(int key);
    ENGINE_API static bool IsUp(int key);
    ENGINE_API static bool IsFree(int key);
    ENGINE_API static unsigned long long UpTime(int key);
    ENGINE_API static const Float2& GetPrevMousePosition();
    ENGINE_API static const Float2& GetCurrentMousePosition();
    ENGINE_API static const Float2& GetDeltaMouseMove();

private:
    unsigned long long isAnyKeyDown_ : 1;
    unsigned long long isAnyKeyPress_ : 1;

    HashTable* hashTable_;

    Float2 prevMousePos_;
    Float2 curMousePos_;
    Float2 deltaMouseMove_;
};
