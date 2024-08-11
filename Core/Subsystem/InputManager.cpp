#include "stdafx.h"
#include "InputManager.h"

// ���콺 ��ǥ �� ���

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
    // ���� ���ѵ��� ����������� �ٸ� �������� ����� ������
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
        reinterpret_cast<void**>(&direct_input_device),     // �����Ͱ� ������ �ִ� �ּҸ� ����
        nullptr
    );

    if (FAILED(hr)) { assert(false); return false; }

    //=========================================
    // [Create Key Device]
    //=========================================

    hr = direct_input_device->CreateDevice(
        GUID_SysKeyboard,   // ������ ��ġ�� ID
        &key_device,        // �ϼ��� ��ġ�� ���� ������
        nullptr
    );
    if (FAILED(hr)) { assert(false); return false; }

    key_device->SetDataFormat(&c_dfDIKeyboard);     // Ű������ ������������ Ű���� ���·� �ްڴ�

    hr = key_device->SetCooperativeLevel(       // ��ġ�� ���� ���� ����
        Settings::Get().GetWindowHandle(),
        DISCL_NONEXCLUSIVE |        // ������ ��ġ�� �����Ӱ� ���� ����
        DISCL_FOREGROUND            // â�� ����� ���� ���� �Է��� �ްڴ�
    );
    if (FAILED(hr)) { assert(false); return false; }

    while (key_device->Acquire() == DIERR_INPUTLOST);       // Ű����κ��� ���ٱ����� ���;� ��. ���� if������ �����ߴٸ� ��������� ���� �� �ְ� while���� �ɾ���

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

    // ������ ����Ǵ� ���� �ٸ� â�� ��� ������ ���� ���� ����
    HRESULT hr = key_device->GetDeviceState(256, input_key_state);
    if (FAILED(hr))
    {
        // ���ӿ� ������ �����Ѵٸ� �ٽ� ���� ������ ���õ�
        while (key_device->Acquire() == DIERR_INPUTLOST);
        // ���� ������ �ٽ� �����Դٸ� �Է��� �ٽ� �޾���
        hr = key_device->GetDeviceState(256, input_key_state);

        if (FAILED(hr)) return;     // �׷��� �����Ѵٸ� �׳� ���Ϲ���(������ ���̷�)
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
    GetCursorPos(&mouse_position);      // ���콺 ��ġ ���
    ScreenToClient(Settings::Get().GetWindowHandle(), &mouse_position);      // Ŭ���̾�Ʈ �������� ���콺 ��ġ ���

    // Ŭ���̾�Ʈ�� ��� ��ǥ�� (0, 0)���� ����
    mouse_position.x -= static_cast<LONG>(Settings::Get().GetWidth() / 2);
    mouse_position.y = -mouse_position.y + static_cast<LONG>(Settings::Get().GetHeight() / 2);

    // ���콺 ������ �� ���
    //system("cls");
    //std::cout << "x : " << mouse_position.x << std::endl;
    //std::cout << "y : " << mouse_position.y << std::endl;


    for (int button = 0; button < 3; button++)
    {
        // �ش� Ű�� ������ �� true
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
        // �ش� Ű�� ������ �� false
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
        // �ش� Ű�� ������ �� true
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
        // �ش� Ű�� ������ �� false
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
