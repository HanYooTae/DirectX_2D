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
	// Position 데이터만 받아옴
	if (ImGui::Begin("Transform"))
	{
		// 1. 출력하고 싶은 데이터들을 받아옴
		auto position = transform->GetPosition();
		//auto scale = transform->GetScale();

		// 2. 데이터 출력. (배열 데이터임 -> call by reference)
		ImGui::InputFloat3("Position", position);		// Vector3니까 InputFloat3
		//ImGui::SliderFloat3("Scale", scale, 0.0f, 300.0f);

		// 3. imgui창에서 변경된 수치 값을 transform에 저장
		transform->SetPosition(position);
		//transform->SetScale(scale);
	}
	ImGui::End();

	// Scale 데이터만 받아옴
	if (ImGui::Begin("Transform"))
	{
		// 1. 출력하고 싶은 데이터들을 받아옴
		//auto position = transform->GetPosition();
		auto scale = transform->GetScale();

		// 2. 데이터 출력. (배열 데이터임 -> call by reference)
		//ImGui::InputFloat3("Position", position);		// Vector3니까 InputFloat3
		ImGui::SliderFloat3("Scale", scale, 0.0f, 300.0f);

		// 3. imgui창에서 변경된 수치 값을 transform에 저장
		//transform->SetPosition(position);
		transform->SetScale(scale);
	}
	ImGui::End();

	// position과 scale의 창의 이름(Transform)이 같기 때문에 따로 구분해도 같이 출력됨

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
