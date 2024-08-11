#include "stdafx.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

void CameraComponent::Initialize()
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Destroy()
{
	// 동적 할당된 데이터가 없으면 굳이 처리안해줘도 됨
}

void CameraComponent::UpdateViewMatrix()
{
	D3DXVECTOR3 position = transform->GetPosition();
	D3DXVECTOR3 at = position + transform->GetForward();		// 전방
	D3DXVECTOR3 up = transform->GetUp();

	D3DXMatrixLookAtLH(&view, &position, &at, &up);
}

void CameraComponent::UpdateProjectionMatrix()
{

	D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0.0f, 1.0f);
}
