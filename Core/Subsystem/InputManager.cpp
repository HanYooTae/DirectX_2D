#include "stdafx.h"
#include "InputManager.h"

// 마우스 좌표 값 계산

InputManager::InputManager(Context* context)
    :ISubsystem(context)
{
    ZeroMemory(input_key_state, sizeof(BYTE) * 256);
    ZeroMemory(&input_mouse_state, sizeof(DIMOUSESTATE));
    
    ZeroMemory(key_state, sizeof(KEY_STATE) * 256);
    ZeroMemory(mouse_state, sizeof(KEY_STATE) * 3);
    ZeroMemory(&mouse_position, sizeof(POINT));
}

InputManager::~InputManager()
{
    // 얻어온 권한들을 해제시켜줘야 다른 곳에서도 사용이 가능함
    if (mouse_device != nullptr)
    {
        mouse_device->Unacquire();
        SAFE_RELEASE(mouse_device);
    }

    if (key_device != nullptr)
    {
        key_device->Unacquire();
        SAFE_RELEASE(key_device);
    }

    SAFE_RELEASE(direct_input_device);
}

bool InputManager::Initialize()
{
    //=========================================
    // [Create Direct Input Device]
    //=========================================

    HRESULT hr = DirectInput8Create
    (
        Settings::Get().GetWindowInstance(),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void**>(&direct_input_device),     // 포인터가 가지고 있는 주소를 변경
        nullptr
    );

    if (FAILED(hr)) { assert(false); return false; }

    //=========================================
    // [Create Key Device]
    //=========================================

    hr = direct_input_device->CreateDevice(
        GUID_SysKeyboard,   // 생성할 장치의 ID
        &key_device,        // 완성된 장치를 받을 포인터
        nullptr
    );
    if (FAILED(hr)) { assert(false); return false; }

    key_device->SetDataFormat(&c_dfDIKeyboard);     // 키보드의 데이터형식을 키보드 형태로 받겠다

    hr = key_device->SetCooperativeLevel(       // 장치에 대한 접근 권한
        Settings::Get().GetWindowHandle(),
        DISCL_NONEXCLUSIVE |        // 윈도우 장치에 자유롭게 접근 가능
        DISCL_FOREGROUND            // 창이 띄워져 있을 때만 입력을 받겠다
    );
    if (FAILED(hr)) { assert(false); return false; }

    while (key_device->Acquire() == DIERR_INPUTLOST);       // 키보드로부터 접근권한을 얻어와야 함. 위의 if문에서 실패했다면 계속적으로 얻어올 수 있게 while문을 걸어줌

    //=========================================
    // [Create Mouse Device]
    //=========================================

    hr = direct_input_device->CreateDevice(
        GUID_SysMouse,
        &mouse_device,
        nullptr
    );
    if (FAILED(hr)) { assert(false); return false; }

    mouse_device->SetDataFormat(&c_dfDIMouse);

    hr = mouse_device->SetCooperativeLevel(
        Settings::Get().GetWindowHandle(),
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND
    );
    if (FAILED(hr)) { assert(false); return false; }

    while (mouse_device->Acquire() == DIERR_INPUTLOST);

    return true;
}

void InputManager::Update()
{
    if (key_device == nullptr || mouse_device == nullptr)
    {
        assert(false);
        return;
    }

    //==================================
    // [Read Data]
    //==================================

    // 게임이 진행되는 동안 다른 창에 잠시 권한일 뺏길 수도 있음
    HRESULT hr = key_device->GetDeviceState(256, input_key_state);
    if (FAILED(hr))
    {
        // 게임에 접근을 실패한다면 다시 접근 권한을 취득시도
        while (key_device->Acquire() == DIERR_INPUTLOST);
        // 접근 권한을 다시 가져왔다면 입력을 다시 받아줌
        hr = key_device->GetDeviceState(256, input_key_state);

        if (FAILED(hr)) return;     // 그래도 실패한다면 그냥 리턴받음(원래는 안이럼)
    }

    hr = mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &input_mouse_state);
    if (FAILED(hr))
    {
        while (mouse_device->Acquire() == DIERR_INPUTLOST);
        hr = mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &input_mouse_state);

        if (FAILED(hr)) return;
    }

    //==================================
    // [Update Mouse Input]
    //==================================
    GetCursorPos(&mouse_position);      // 마우스 위치 출력
    ScreenToClient(Settings::Get().GetWindowHandle(), &mouse_position);      // 클라이언트 기준으로 마우스 위치 출력

    // 클라이언트의 가운데 좌표를 (0, 0)으로 변경
    mouse_position.x -= static_cast<LONG>(Settings::Get().GetWidth() / 2);
    mouse_position.y = -mouse_position.y + static_cast<LONG>(Settings::Get().GetHeight() / 2);

    // 마우스 포인터 값 출력
    //system("cls");
    //std::cout << "x : " << mouse_position.x << std::endl;
    //std::cout << "y : " << mouse_position.y << std::endl;


    for (int button = 0; button < 3; button++)
    {
        // 해당 키가 눌렸을 때 true
        if (input_mouse_state.rgbButtons[button] & 0x80)
        {
            switch (mouse_state[button])
            {
            case KEY_FREE:
            case KEY_UP:
                mouse_state[button] = KEY_DOWN;
                break;
            case KEY_DOWN:
            case KEY_HOLD:
                mouse_state[button] = KEY_HOLD;
                break;
            }
        }
        // 해당 키가 눌렸을 때 false
        else
        {
            switch (mouse_state[button])
            {
            case KEY_FREE:
            case KEY_UP:
                mouse_state[button] = KEY_FREE;
                break;
            case KEY_DOWN:
            case KEY_HOLD:
                mouse_state[button] = KEY_UP;
                break;
            }
        }
    }

    //==================================
    // [Update Key Input]
    //==================================

    for (int key = 0; key < 256; key++)
    {
        // 해당 키가 눌렸을 때 true
        if (input_key_state[key] & 0x80)
        {
            switch (key_state[key])
            {
            case KEY_FREE:
            case KEY_UP:
                key_state[key] = KEY_DOWN;
                break;
            case KEY_DOWN:
            case KEY_HOLD:
                key_state[key] = KEY_HOLD;
                break;
            }
        }
        // 해당 키가 눌렸을 때 false
        else
        {
            switch (key_state[key])
            {
            case KEY_FREE:
            case KEY_UP:
                key_state[key] = KEY_FREE;
                break;
            case KEY_DOWN:
            case KEY_HOLD:
                key_state[key] = KEY_UP;
                break;
            }
        }
    }
}
