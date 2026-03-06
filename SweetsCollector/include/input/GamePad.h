//-----------------------------------------------
// GamePad.h
// 制作日：2026/02/16
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#pragma once
#include <Windows.h>
#include <array>
#include <cstdint>

struct VECTOR2 { float x{}, y{}; };

enum class PAD_BUTTON : uint8_t
{
    A, B, X, Y,
    LB, RB,
    Back, Start,
    LStick, RStick,
    DpadUp, DpadDown, DpadLeft, DpadRight,
};

class GAME_PAD
{
private:
    VECTOR2 m_ls{}, m_rs{};
    float m_lt{}, m_rt{};

    std::array<uint8_t, 32> m_btn{};
    std::array<uint8_t, 32> m_prev{};

    int m_pov = -1;
    bool m_connected = false;

    float m_deadL = 0.18f;
    float m_deadR = 0.18f;

public:
    void UpdateFromRaw(const struct DIJOYSTATE2& s);

    bool Connected() const { return m_connected; }
    void SetConnected(bool v) { m_connected = v; }

    VECTOR2 LeftStick() const { return m_ls; }
    VECTOR2 RightStick() const { return m_rs; }

    float LeftTrigger()  const { return m_lt; }
    float RightTrigger() const { return m_rt; }

    bool Down(PAD_BUTTON b) const;
    bool Pressed(PAD_BUTTON b) const;
    bool Released(PAD_BUTTON b) const;

    // 調整
    void SetDeadzone(float left, float right) { m_deadL = left; m_deadR = right; }
};

class INPUT_SYSTEM
{
private:
    HWND m_hwnd{};
    bool m_app_active = true;

    struct DIContext;
    DIContext* m_di = nullptr;

    std::array<GAME_PAD, 1> m_pads;

private:
    INPUT_SYSTEM() = default;
    INPUT_SYSTEM(const INPUT_SYSTEM&) = delete;
    INPUT_SYSTEM& operator=(const INPUT_SYSTEM&) = delete;

    bool InitDirectInput(HINSTANCE hInst, HWND hwnd);
    void ShutdownDirectInput();
    void Poll();

public:
    static INPUT_SYSTEM& Instance();

    bool Init(HINSTANCE hInst, HWND hwnd);
    void Uninit();
    void Update();

    GAME_PAD& GetGamePad(int idx = 0) { return m_pads[idx]; }

    void OnAppActivate(bool active);
};
