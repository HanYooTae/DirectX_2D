#include "stdafx.h"
#include "TestComponent.h"

#include "Scene/Actor.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

void TestComponent::Initialize()
{
}

void TestComponent::Update()
{
	// Position �����͸� �޾ƿ�
	if (ImGui::Begin("Transform"))
	{
		// 1. ����ϰ� ���� �����͵��� �޾ƿ�
		auto position = transform->GetPosition();
		//auto scale = transform->GetScale();

		// 2. ������ ���. (�迭 �������� -> call by reference)
		ImGui::InputFloat3("Position", position);		// Vector3�ϱ� InputFloat3
		//ImGui::SliderFloat3("Scale", scale, 0.0f, 300.0f);

		// 3. imguiâ���� ����� ��ġ ���� transform�� ����
		transform->SetPosition(position);
		//transform->SetScale(scale);
	}
	ImGui::End();

	// Scale �����͸� �޾ƿ�
	if (ImGui::Begin("Transform"))
	{
		// 1. ����ϰ� ���� �����͵��� �޾ƿ�
		//auto position = transform->GetPosition();
		auto scale = transform->GetScale();

		// 2. ������ ���. (�迭 �������� -> call by reference)
		//ImGui::InputFloat3("Position", position);		// Vector3�ϱ� InputFloat3
		ImGui::SliderFloat3("Scale", scale, 0.0f, 300.0f);

		// 3. imguiâ���� ����� ��ġ ���� transform�� ����
		//transform->SetPosition(position);
		transform->SetScale(scale);
	}
	ImGui::End();

	// position�� scale�� â�� �̸�(Transform)�� ���� ������ ���� �����ص� ���� ��µ�

	if (ImGui::Begin("Actor"))
	{
		
		auto renderer = actor->GetComponent<MeshRendererComponent>();
		auto is_renderer = renderer->IsEnabled();

		ImGui::Checkbox("Render", &is_renderer);

		renderer->SetEnabled(is_renderer);
	}
	ImGui::End();
}

void TestComponent::Destroy()
{
}
