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

	// Async : update�� ȣ��� �������κ��� �Ǵ���. ������ �Һи�
	//if (GetAsyncKeyState('W') & 0x8000) position.y+=3;
	//if (GetAsyncKeyState('S') & 0x8000) position.y-=3;
	//if (GetAsyncKeyState('D') & 0x8000) position.x+=3;
	//if (GetAsyncKeyState('A') & 0x8000) position.x-=3;

	if (input_manager->GetKeyState(DIK_W) & (KEY_DOWN | KEY_HOLD)) position.y += 3;
	if (input_manager->GetKeyState(DIK_S) & (KEY_DOWN | KEY_HOLD)) position.y -= 3;
	if (input_manager->GetKeyState(DIK_D) & (KEY_DOWN | KEY_HOLD)) position.x += 3;
	if (input_manager->GetKeyState(DIK_A) & (KEY_DOWN | KEY_HOLD)) position.x -= 3;

	// Meshrenderer�� ���콺�� ����ٴϰ� �ϴ� �ڵ�
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
      - Immediate Mode GUI(��� ��� �׷��� ����� �������̽�)
      - ���� ������ ����� �������̽��� ���� �� �ִ� ���̺귯��
      - ����� �뵵�� ���� �����
*/