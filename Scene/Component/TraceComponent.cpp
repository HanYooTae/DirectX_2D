#include "stdafx.h"
#include "TraceComponent.h"
#include "TransformComponent.h"
#include "Scene/Actor.h"

void TraceComponent::Initialize()
{
}

void TraceComponent::Update()	// ������Ʈ�� �ʴ� 50�� ȣ��Ǹ� ���ǵ带 40���� ����� �� 40��ŭ �����̴� ���� �ƴ� 200�� �����̴� ���� �ȴ�.
{
	// Ÿ���� ���ٸ� ����
	if (target.expired() == true) return;

	D3DXVECTOR3 my_position = transform->GetPosition();

	// weak_ptr�� �ٷ� ����. ����ϴٰ� �����Ͱ� ����� ���� �ֱ� ������ lock()�� ��� -> shared_ptr�� ����
	D3DXVECTOR3 target_position = target.lock()->GetTransform()->GetPosition();

	/*
		���� - �÷��̾� = �÷��̾� -> ����
		�÷��̾� - ���� = ���� -> �÷��̾�
	*/ 


	// �Ѿƿ��� ������ �����
	// x���� �������� y������ �����
	// y���� �������� x������ �����
	/*if (my_position.x > target_position.x) my_position.x--;
	else my_position.x++;

	if (my_position.y > target_position.y) my_position.y--;
	else my_position.y++;*/

	/*
		A - B = B���� A�� ���ϴ� ����
		-> B���� A�� ���ϴ� ���Ͱ��� ���� �� ����
	*/
	D3DXVECTOR3 dir = target_position - my_position;
	D3DXVECTOR3 dir1;

	// ��Ÿ����� ������ �̿��� ���� ���ϱ�
	// ����(����)^2 = �غ�^2 + ����^2 
	// -> �����^2 = -> ��(�غ�^2 + ����^2)
	// -> ���� = ��(�غ�^2 + ����^2)
	// D3DXVec3Length ����ϸ� ��

	/*float length = sqrt(dir.x * dir.x + dir.y * dir.y);
	system("cls");
	printf("distance : %f \n", length);

	if (length >= 300) current_state = State::Trace;
	if (length <= 300) current_state = State::Avoid;*/


	auto length = D3DXVec3Length(&dir);
	system("cls");
	printf("distance : %f \n", length);
	
	if (length >= 300) current_state = State::Trace;
	if (length <= 300) current_state = State::Avoid;

	
	/*
		���� dir�� target���� ���ϴ� ���������� ���� ���� ũ�Ⱑ ������ ����
		-> ���� ���ͷ� ����� ������ ���� ������ �ٲ۴�.
	*/
	D3DXVec3Normalize(&dir1, &dir);		// 1�� ����ȭ (���� ����ȭ)

	// �ڽ��� ��ġ + (��������(����) * speed * ��ŸŸ��(�ʴ���))���� ����ϸ�
	// ������Ʈ Ƚ���� �ƴ� �ð� ������ �°� ������ �� ����
	// (-> �ڽ��� ��ġ���� �ش� �������� speed��ŭ �̵��� ��ġ�� ����)
	//my_position += dir * speed * timer.GetElapsedTimeSec();

	D3DXVECTOR3 move_position;

	switch (current_state)
	{
	case Trace:
		// �ڽ��� ��ġ + (�������� * speed * ��ŸŸ��(�ʴ���))���� ����ϸ�
		// ������Ʈ Ƚ���� �ƴ� �ð� ������ �°� ������ �� ����
		move_position = my_position + dir * (speed * timer.GetElapsedTimeSec());
		std::cout << "State : Chase" << std::endl;
		std::cout << "Direction : ( " << dir.x << ", " << dir.y << " )" << std::endl;
		break;

	case Avoid:
		// ������ �������־� �÷��̾�κ��� �־�������
		move_position = my_position - dir * (speed * timer.GetElapsedTimeSec());
		std::cout << "State : Avoid" << std::endl;
		std::cout << "Direction : ( " << -dir.x << ", " << -dir.y << " )" << std::endl;
		break;
	}

	transform->SetPosition(move_position);

	timer.Start();
}

void TraceComponent::Destroy()
{
}
