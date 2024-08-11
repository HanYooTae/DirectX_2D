#include "stdafx.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(class Context* context, Actor* actor, TransformComponent* transform)
	: IComponent(context, actor, transform)
{
	D3DXMatrixIdentity(&local);
	D3DXMatrixIdentity(&world);
	UpdateTransform();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Update()
{
}

void TransformComponent::Destroy()
{
	childs.clear();
}

void TransformComponent::SetLocalScale(const D3DXVECTOR3& scale)
{
	if (this->local_scale == scale) return;

	this->local_scale = scale;
	UpdateTransform();
}

void TransformComponent::SetLocalPosition(const D3DXVECTOR3& position)
{
	if (this->local_position == position) return;

	this->local_position = position;
	UpdateTransform();
}

void TransformComponent::SetLocalRotation(const D3DXVECTOR3& rotation)
{
	if (this->local_rotation == rotation) return;

	this->local_rotation = rotation;
	UpdateTransform();
}

const D3DXVECTOR3 TransformComponent::GetScale()
{
	D3DXVECTOR3 world_scale;

	/*
		오일러각 : 강체의 방향을 3차원 공간에 표시하기 위해 레온하르트 오일러가 도입한 3개의 각도(x, y, z)

		과제 : 오일러각, 짐벌락, 쿼터니언 조사
	*/

	D3DXQUATERNION temp1;
	D3DXVECTOR3 temp2;

	D3DXMatrixDecompose(&world_scale, &temp1, &temp2, &world);

	return world_scale;
}

void TransformComponent::SetScale(const D3DXVECTOR3& world_scale)
{
	if (GetScale() == world_scale) return;

	if (HasParent() == true)
	{
		/*
			로컬 : 0.5, 0.5, 1.0
			부모 : 4.0, 4.0, 1.0
			월드 : 2.0, 2.0, 1.0

			world_scale 값을 2.0, 2.0, 1.0을 바로 넣어준다면 로컬 값은 2.0, 2.0, 1.0

			로컬 : 2.0, 2.0, 1.0
			부모 : 4.0, 4.0, 1.0
			월드 : 8.0, 8.0, 1.0

			부모 값이 곱해지게 된다면 월드 크기가 2.0, 2.0, 1.0이 아니게 됨
		*/


		/*
			월드s = 자식s * 부모s
			-> 월드s / 부모s = 자식s * 부모s / 부모s
			-> 월드s / 부모s = 자식s

			월드에 표현되는 자신의 크기는 부모의 행렬과 결합된 결과이기 때문에
			부모가 있을 경우 입력받은 월드 스케일 값을 그대로 적용시켜주면 문제가 생긴다.
			따라서 스케일 값을 정해줄 때 월드 스케일에서 부모의 스케일 값을 나눠
			자신의 스케일 값을 정해주어야 한다.
		*/
		D3DXVECTOR3 scale;
		D3DXVECTOR3 parent_scale = parent->GetScale();
		scale.x = world_scale.x / parent_scale.x;
		scale.y = world_scale.y / parent_scale.y;
		scale.z = world_scale.z / parent_scale.z;

		SetLocalScale(scale);
	}
	else
		SetLocalScale(world_scale);
}

const D3DXVECTOR3 TransformComponent::GetPosition()
{
	D3DXVECTOR3 world_position;

	D3DXQUATERNION temp1;
	D3DXVECTOR3 temp2;

	// D3DXMatrixDecompose() 순서 :크기, 자전, 위치
	D3DXMatrixDecompose(&temp2, &temp1, &world_position, &world);

	return world_position;
}

void TransformComponent::SetPosition(const D3DXVECTOR3& world_position)
{
	if (GetPosition() == world_position) return;

	if (HasParent() == true)
	{
		// 월드 = 자식 * 부모
		// 부모 * 부모의 역행렬 = 1(단위행렬);

		D3DXMATRIX inverse;
		D3DXMatrixInverse(&inverse, nullptr, &parent->GetWorldMatrix());

		// 1*3, 4*4

		D3DXVECTOR3 position;
		D3DXVec3TransformCoord(&position, &world_position, &inverse);	// 1*3을 1*4로 늘려줌

		SetLocalPosition(position);
	}
	else
		SetLocalPosition(world_position);
}

const D3DXVECTOR3 TransformComponent::GetRotation()
{
	D3DXQUATERNION world_rotation;
	D3DXVECTOR3 temp2;

	D3DXMatrixDecompose(&temp2, &world_rotation, &temp2, &world);

	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation);

	return D3DXVECTOR3	// Y -> X -> Z 순서대로 회전 데이터를 합쳐줄 예정
	(
		atan2(rotation._31, rotation._33),
		atan2(-rotation._32, sqrt(pow(rotation._12, 2) + pow(rotation._22, 2))),
		atan2(rotation._12, rotation._22)
	);
}

void TransformComponent::SetRotation(const D3DXVECTOR3& world_rotation)
{
	if (GetRotation() == world_rotation) return;

	if (HasParent() == true)
	{
		D3DXMATRIX inverse;
		D3DXMATRIX my_world_rotation = GetWorldRotationMatrix();

		D3DXMatrixInverse(&inverse, nullptr, &my_world_rotation);

		D3DXVECTOR3 rotation;
		D3DXVec3TransformNormal(&rotation, &world_rotation, &inverse);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(world_rotation);
}

const D3DXMATRIX TransformComponent::GetWorldRotationMatrix()
{
	D3DXQUATERNION world_rotation;
	D3DXVECTOR3 temp;

	D3DXMatrixDecompose(&temp, &world_rotation, &temp, &world);

	D3DXMATRIX rotation;
	D3DXMatrixRotationQuaternion(&rotation, &world_rotation);

	return rotation;
}

const D3DXVECTOR3 TransformComponent::GetRight() const
{
	D3DXVECTOR3 right;
	D3DXVECTOR3 vec(1, 0, 0);

	D3DXVec3TransformNormal(&right, &vec, &world);

	return right;
}

const D3DXVECTOR3 TransformComponent::GetUp() const
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 vec(0, 1, 0);

	D3DXVec3TransformNormal(&up, &vec, &world);

	return up;
}

const D3DXVECTOR3 TransformComponent::GetForward() const
{
	D3DXVECTOR3 forward;
	D3DXVECTOR3 vec(0, 0, 1);

	D3DXVec3TransformNormal(&forward, &vec, &world);

	return forward;
}

void TransformComponent::UpdateTransform()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, local_scale.x, local_scale.y, local_scale.z);

	D3DXMatrixRotationYawPitchRoll(&R, local_rotation.y, local_rotation.x, local_rotation.z);
	D3DXMatrixTranslation(&T, local_position.x, local_position.y, local_position.z);

	local = S * R * T;

	if (HasParent() == true)
		world = local * parent->GetWorldMatrix();
	else
		world = local;

	// 부모가 변경되면 자식도 변경
	for (const auto& child : childs)
		child->UpdateTransform();
}

void TransformComponent::SetParent(TransformComponent* const new_parent)
{
	if (new_parent == this)
		return;

	parent = new_parent;

	new_parent->AddChild(this);
}

TransformComponent* const TransformComponent::GetChildFromIndex(const uint& index)
{
	if (HasChilds() == false) return nullptr;
	if (index >= GetChildCount()) return nullptr;

	return childs[index];
}

void TransformComponent::AddChild(TransformComponent* const child)
{
	if (child == this) return;

	childs.emplace_back(child);
}

void TransformComponent::DeleteChild(TransformComponent* const child)
{
	for (auto iter = childs.begin(); iter != childs.end(); iter++)
	{
		if (*iter == child)
		{
			childs.erase(iter);
		}
	}

}
