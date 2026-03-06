//-----------------------------------------------
// GamePad.cpp
// 制作日：2026/02/16
// 制作者：安田晴人
//-----------------------------------------------
#define DIRECTINPUT_VERSION 0x0800
#include "input\\GamePad.h"
#include <dinput.h>
#include <algorithm>
#include <cmath>

//-----------------------------------------------
// DirectInput Context
//-----------------------------------------------
struct INPUT_SYSTEM::DIContext
{
    IDirectInput8* di = nullptr;
    IDirectInputDevice8* joy = nullptr;
    DIJOYSTATE2 state{};
};

//-----------------------------------------------
// クランプ
//-----------------------------------------------
static float Clamp(float v, float a, float b)
{
	if (v < a) return a;
	if (v > b) return b;
	return v;
}

//-----------------------------------------------
// 軸値を -1..1 に正規化
//-----------------------------------------------
static float NormalizeAxis(LONG v)
{
    if (v < 0) return (float)v / 32768.0f;
    return (float)v / 32767.0f;
}

//-----------------------------------------------
// デッドゾーン適用
//-----------------------------------------------
static float ApplyDeadzone(float v, float dz)
{
    float a = std::fabs(v);
    if (a <= dz) return 0.0f;
    float sign = (v < 0) ? -1.0f : 1.0f;
    float t = (a - dz) / (1.0f - dz);
    return sign * Clamp(t, 0.0f, 1.0f);
}

static bool SetAxisRange(IDirectInputDevice8* dev, DWORD dwOfs)
{
    DIPROPRANGE pr{};
    pr.diph.dwSize = sizeof(DIPROPRANGE);
    pr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    pr.diph.dwHow = DIPH_BYOFFSET;
    pr.diph.dwObj = dwOfs;
    pr.lMin = -32768;
    pr.lMax = 32767;
    return SUCCEEDED(dev->SetProperty(DIPROP_RANGE, &pr.diph));
}


static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* obj, VOID* ctx)
{
    auto* dev = reinterpret_cast<IDirectInputDevice8*>(ctx);

    if (obj->dwType & DIDFT_AXIS)
    {
        // GUIDじゃなく dwOfs を使う
        SetAxisRange(dev, obj->dwOfs);
    }
    return DIENUM_CONTINUE;
}

// EnumDevices用のコンテキスト
struct EnumJoyCtx
{
    IDirectInput8* di = nullptr;
    IDirectInputDevice8* joy = nullptr;
};

static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* inst, VOID* ctx)
{
    auto* c = reinterpret_cast<EnumJoyCtx*>(ctx);
    if (c->joy) return DIENUM_STOP;

    if (SUCCEEDED(c->di->CreateDevice(inst->guidInstance, &c->joy, nullptr)))
        return DIENUM_STOP;

    return DIENUM_CONTINUE;
}

//------------------------------
// Singleton
//------------------------------
INPUT_SYSTEM& INPUT_SYSTEM::Instance()
{
    static INPUT_SYSTEM s;
    return s;
}

//------------------------------
// INPUT_SYSTEM
//------------------------------
bool INPUT_SYSTEM::Init(HINSTANCE hInst, HWND hwnd)
{
    m_hwnd = hwnd;
    return InitDirectInput(hInst, hwnd);
}

void INPUT_SYSTEM::Uninit()
{
    ShutdownDirectInput();
}

void INPUT_SYSTEM::Update()
{
    Poll();
}

void INPUT_SYSTEM::OnAppActivate(bool active)
{
    m_app_active = active;

    if (!m_di || !m_di->joy) return;

    if (active)
    {
        m_di->joy->Acquire();
    }
    else
    {
        m_di->joy->Unacquire();
    }
}

bool INPUT_SYSTEM::InitDirectInput(HINSTANCE hInst, HWND hwnd)
{
    m_di = new DIContext();

    if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_di->di, nullptr)))
        return false;

    EnumJoyCtx ctx{};
    ctx.di = m_di->di;
    m_di->di->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, &ctx, DIEDFL_ATTACHEDONLY);
    m_di->joy = ctx.joy;

    if (!m_di->joy)
    {
        m_pads[0].SetConnected(false);
        return true; // 未接続でも起動継続
    }

    if (FAILED(m_di->joy->SetDataFormat(&c_dfDIJoystick2))) return false;
    if (FAILED(m_di->joy->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;

    // 軸範囲を統一
    m_di->joy->EnumObjects(EnumObjectsCallback, m_di->joy, DIDFT_AXIS);

    m_di->joy->Acquire();
    m_pads[0].SetConnected(true);
    return true;
}

void INPUT_SYSTEM::ShutdownDirectInput()
{
    if (!m_di) return;

    if (m_di->joy)
    {
        m_di->joy->Unacquire();
        m_di->joy->Release();
        m_di->joy = nullptr;
    }
    if (m_di->di)
    {
        m_di->di->Release();
        m_di->di = nullptr;
    }
    delete m_di;
    m_di = nullptr;
}

void INPUT_SYSTEM::Poll()
{
    if (!m_di || !m_di->joy || !m_app_active)
    {
        m_pads[0].SetConnected(false);
        return;
    }

    HRESULT hr = m_di->joy->Poll();
    if (FAILED(hr))
    {
        hr = m_di->joy->Acquire();
        while (hr == DIERR_INPUTLOST) hr = m_di->joy->Acquire();

        if (FAILED(hr))
        {
            m_pads[0].SetConnected(false);
            return;
        }
    }

    if (FAILED(m_di->joy->GetDeviceState(sizeof(DIJOYSTATE2), &m_di->state)))
    {
        m_pads[0].SetConnected(false);
        return;
    }

    m_pads[0].SetConnected(true);
    m_pads[0].UpdateFromRaw(m_di->state);
}

//------------------------------
// GAME_PAD mapping
//------------------------------
static int Pov4Dir(int pov100)
{
    if (pov100 < 0) return -1;
    int deg = pov100 / 100;
    if (deg == 0)   return 0; // up
    if (deg == 90)  return 1; // right
    if (deg == 180) return 2; // down
    if (deg == 270) return 3; // left
    return -1;
}

static int ButtonToIndex(PAD_BUTTON b)
{
    // Xbox系は環境によりズレるので、必要なら後で「設定」で差し替えできるようにすると強い
    switch (b)
    {
    case PAD_BUTTON::A: return 0;
    case PAD_BUTTON::B: return 1;
    case PAD_BUTTON::X: return 2;
    case PAD_BUTTON::Y: return 3;
    case PAD_BUTTON::LB: return 4;
    case PAD_BUTTON::RB: return 5;
    case PAD_BUTTON::Back: return 6;
    case PAD_BUTTON::Start: return 7;
    case PAD_BUTTON::LStick: return 8;
    case PAD_BUTTON::RStick: return 9;
    default: return -1;
    }
}

void GAME_PAD::UpdateFromRaw(const DIJOYSTATE2& s)
{
    m_prev = m_btn;

    // stick（一般的な割り当て例）
    float lx = ApplyDeadzone(NormalizeAxis(s.lX), m_deadL);
    float ly = ApplyDeadzone(NormalizeAxis(s.lY), m_deadL);
    float rx = ApplyDeadzone(NormalizeAxis(s.lRx), m_deadR);
    float ry = ApplyDeadzone(NormalizeAxis(s.lRy), m_deadR);

    m_ls = { lx, -ly };
    m_rs = { rx, -ry };

    for (int i = 0; i < 32; ++i)
        m_btn[i] = (s.rgbButtons[i] & 0x80) ? 1 : 0;

    m_pov = (int)s.rgdwPOV[0];

    // Trigger（ここがDirectInput最大の差分ポイント）
    // まずは「lZ を 0..1」で読む仮実装。
    // 左右別にしたいなら “実機でどの軸が動くか” をログして分岐を作る。
    float z01 = Clamp((s.lZ + 32768.0f) / 65535.0f, 0.0f, 1.0f);
    m_lt = 0.0f;
    m_rt = z01;
}

bool GAME_PAD::Down(PAD_BUTTON b) const
{
    // DPad
    if (b == PAD_BUTTON::DpadUp || b == PAD_BUTTON::DpadRight || b == PAD_BUTTON::DpadDown || b == PAD_BUTTON::DpadLeft)
    {
        int idx = Pov4Dir(m_pov);
        if (idx < 0) return false;
        if (b == PAD_BUTTON::DpadUp)    return idx == 0;
        if (b == PAD_BUTTON::DpadRight) return idx == 1;
        if (b == PAD_BUTTON::DpadDown)  return idx == 2;
        return idx == 3;
    }

    int i = ButtonToIndex(b);
    if (i < 0 || i >= 32) return false;
    return m_btn[i] != 0;
}

bool GAME_PAD::Pressed(PAD_BUTTON b) const
{
    int i = ButtonToIndex(b);
    if (i < 0 || i >= 32) return false;
    return (m_btn[i] != 0) && (m_prev[i] == 0);
}

bool GAME_PAD::Released(PAD_BUTTON b) const
{
    int i = ButtonToIndex(b);
    if (i < 0 || i >= 32) return false;
    return (m_btn[i] == 0) && (m_prev[i] != 0);
}
