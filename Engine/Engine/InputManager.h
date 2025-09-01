#pragma once

class HashTable;
class InputManager final {
private:
    friend class Engine;

    class InputState {
        friend class InputManager;

    public:
        InputState();

        InputState(int key);

        void InputStateCheck(unsigned long long deltaTick);

    private:
        bool down_ = false;   // 누른 순간
        bool press_ = false;  // 계속 눌려지고있는지
        bool up_ = false;     // 방금 떼어졌는지?
        bool free_ = true;    // 눌려지고 있지 않음.

        int key_ = -1;

        unsigned long long pressTime_ = 0;
        unsigned long long upTime_ = 0;
    };

public:
    InputManager();

    ~InputManager();

    ENGINE_API static InputManager* Instance();

    ENGINE_API bool IsAnyKeyDown() const;

    ENGINE_API bool IsAnyKeyPress() const;

    ENGINE_API bool IsDown(int key);

    ENGINE_API bool IsPress(int key);

    ENGINE_API unsigned long long PressTime(int key);

    ENGINE_API bool IsUp(int key);

    ENGINE_API bool IsFree(int key);

    ENGINE_API unsigned long long UpTime(int key);

    ENGINE_API const Float2& GetPrevMousePosition() const;

    ENGINE_API const Float2& GetCurrentMousePosition() const;

    ENGINE_API const Float2& GetDeltaMouseMove() const;

private:
    bool Initialize();

    void Tick(double deltaTime);

    void UpdateInputState(double deltaTime);

    void UpdateMouseMove();

    unsigned long long isAnyKeyDown_ : 1;

    unsigned long long isAnyKeyPress_ : 1;

    HashTable* hashTable_;

    Float2 prevMousePos_;

    Float2 curMousePos_;

    Float2 deltaMouseMove_;
};
