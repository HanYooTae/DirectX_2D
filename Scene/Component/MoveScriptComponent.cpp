#include "stdafx.h"
#include "MoveScriptComponent.h"
#include "TransformComponent.h"
void MoveScriptComponent::Initialize()
{
}

void MoveScriptComponent::Update()
{
	D3DXVECTOR3 position = transform->GetPosition();

	auto input_manager = context->GetSubsystem<InputManager>();

	// Async : update가 호출된 시점으로부터 판단함. 시점이 불분명
	//if (GetAsyncKeyState('W') & 0x8000) position.y+=3;
	//if (GetAsyncKeyState('S') & 0x8000) position.y-=3;
	//if (GetAsyncKeyState('D') & 0x8000) position.x+=3;
	//if (GetAsyncKeyState('A') & 0x8000) position.x-=3;

	if (input_manager->GetKeyState(DIK_W) & (KEY_DOWN | KEY_HOLD)) position.y += 3;
	if (input_manager->GetKeyState(DIK_S) & (KEY_DOWN | KEY_HOLD)) position.y -= 3;
	if (input_manager->GetKeyState(DIK_D) & (KEY_DOWN | KEY_HOLD)) position.x += 3;
	if (input_manager->GetKeyState(DIK_A) & (KEY_DOWN | KEY_HOLD)) position.x -= 3;

	// Meshrenderer가 마우스를 따라다니게 하는 코드
	//position.x = input_manager->GetMousePosition().x;
	//position.y = input_manager->GetMousePosition().y;
	//input_manager->GetMousePosition();

	transform->SetPosition(position);
}

void MoveScriptComponent::Destroy()
{
}

/*
	ImGui
      - Immediate Mode GUI(즉시 모드 그래픽 사용자 인터페이스)
      - 쉽고 빠르게 사용자 인터페이스를 만들 수 있는 라이브러리
      - 디버깅 용도로 많이 사용함
*/