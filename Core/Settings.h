#pragma once
#include"stdafx.h"

// Settings.h

class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	HWND GetWindowHandle() { return handle; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }

	HINSTANCE GetWindowInstance() { return instance; }
	void SetWindowInstance(HINSTANCE instance) { this->instance = instance; }

	const float& GetWidth() { return width; }
	void SetWidth(const float& width) { this->width = width; }

	const float& GetHeight() { return height; }
	void SetHeight(const float& height) { this->height = height; }

private:
	// default
	Settings() = default;
	~Settings() = default;

	// delete
	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;

private:
	HWND handle = nullptr;
	// 창의 실체, dinput이 instance를 요구함에 따라 추가해줌
	HINSTANCE instance = nullptr;

	float width = 0.0f;
	float height = 0.0f;
};

