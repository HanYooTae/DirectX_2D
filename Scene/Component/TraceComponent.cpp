#include "stdafx.h"
#include "TraceComponent.h"
#include "TransformComponent.h"
#include "Scene/Actor.h"

void TraceComponent::Initialize()
{
}

void TraceComponent::Update()	// 업데이트가 초당 50번 호출되면 스피드를 40으로 잡았을 때 40만큼 움직이는 것이 아닌 200번 움직이는 것이 된다.
{
	// 타겟이 없다면 리턴
	if (target.expired() == true) return;

	D3DXVECTOR3 my_position = transform->GetPosition();

	// weak_ptr는 바로 못씀. 사용하다가 데이터가 사라질 수도 있기 때문에 lock()을 사용 -> shared_ptr로 변경
	D3DXVECTOR3 target_position = target.lock()->GetTransform()->GetPosition();

	/*
		몬스터 - 플레이어 = 플레이어 -> 몬스터
		플레이어 - 몬스터 = 몬스터 -> 플레이어
	*/ 


	// 쫓아오는 방향이 어색함
	// x축이 같아지면 y축으로 따라옴
	// y축이 같아지면 x축으로 따라옴
	/*if (my_position.x > target_position.x) my_position.x--;
	else my_position.x++;

	if (my_position.y > target_position.y) my_position.y--;
	else my_position.y++;*/

	/*
		A - B = B에서 A로 향하는 백터
		-> B에서 A로 향하는 백터값을 구할 수 있음
	*/
	D3DXVECTOR3 dir = target_position - my_position;
	D3DXVECTOR3 dir1;

	// 피타고라스의 정리를 이용한 길이 구하기
	// 길이(빗변)^2 = 밑변^2 + 높이^2 
	// -> √빗변^2 = -> √(밑변^2 + 높이^2)
	// -> 빗변 = √(밑변^2 + 높이^2)
	// D3DXVec3Length 사용하면 됨

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
		현재 dir이 target으로 향하는 벡터이지만 방향 값에 크기가 더해진 상태
		-> 단위 벡터로 만들어 순수한 방향 값으로 바꾼다.
	*/
	D3DXVec3Normalize(&dir1, &dir);		// 1로 정규화 (단위 벡터화)

	// 자신의 위치 + (단위벡터(방향) * speed * 델타타임(초단위))으로 계산하면
	// 업데이트 횟수가 아닌 시간 비율에 맞게 움직일 수 있음
	// (-> 자신의 위치에서 해당 방향으로 speed만큼 이동한 위치가 나옴)
	//my_position += dir * speed * timer.GetElapsedTimeSec();

	D3DXVECTOR3 move_position;

	switch (current_state)
	{
	case Trace:
		// 자신의 위치 + (단위벡터 * speed * 델타타임(초단위))으로 계산하면
		// 업데이트 횟수가 아닌 시간 비율에 맞게 움직일 수 있음
		move_position = my_position + dir * (speed * timer.GetElapsedTimeSec());
		std::cout << "State : Chase" << std::endl;
		std::cout << "Direction : ( " << dir.x << ", " << dir.y << " )" << std::endl;
		break;

	case Avoid:
		// 방향을 뒤집어주어 플레이어로부터 멀어지게함
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
