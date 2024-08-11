#pragma once
#include "ISubsystem.h"

enum KEY_STATE
{
	KEY_FREE,
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD
};

enum MOUSE_BUTTON
{
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
};

class InputManager final : public ISubsystem
{
public:
	InputManager(Context* context);
	~InputManager();

	// Inherited via ISubsystem
	virtual bool Initialize() override;
	virtual void Update() override;

	const POINT& GetMousePosition() const { return mouse_position; }

	const KEY_STATE& GetKeyState(BYTE key) const { return key_state[key]; }
	const KEY_STATE& GetMouseState(MOUSE_BUTTON button) const { return mouse_state[button]; }

private:
	LPDIRECTINPUT8 direct_input_device = nullptr;
	LPDIRECTINPUTDEVICE8 key_device = nullptr;
	LPDIRECTINPUTDEVICE8 mouse_device = nullptr;

	BYTE input_key_state[256];
	DIMOUSESTATE input_mouse_state;

	KEY_STATE key_state[256];
	KEY_STATE mouse_state[3];

	POINT mouse_position;
};

