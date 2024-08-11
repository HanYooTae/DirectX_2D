#pragma once
#include "IComponent.h"


// world관리
class TransformComponent final : public IComponent
{
public:
	TransformComponent
	(
		class Context* context,
		class Actor* actor,
		class TransformComponent* transform
	);
	
	~TransformComponent();

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	//====================================================
	//  [local]
	//====================================================
	const D3DXVECTOR3& GetLocalScale() const { return local_scale; }
	void SetLocalScale(const D3DXVECTOR3& scale);

	const D3DXVECTOR3& GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const D3DXVECTOR3& position);

	const D3DXVECTOR3& GetLocalRotation() const { return local_rotation; }
	void SetLocalRotation(const D3DXVECTOR3& rotation);

	const D3DXMATRIX& GetLocalMatrix() const { return local; }	// 본인과 관련

	//====================================================
	//  [world]
	//====================================================
	const D3DXVECTOR3 GetScale();
	void SetScale(const D3DXVECTOR3& world_scale);

	const D3DXVECTOR3 GetPosition();
	void SetPosition(const D3DXVECTOR3& world_position);

	const D3DXVECTOR3 GetRotation();
	void SetRotation(const D3DXVECTOR3& world_rotation);

	const D3DXMATRIX GetWorldRotationMatrix();
	const D3DXMATRIX& GetWorldMatrix() const { return world; }

	const D3DXVECTOR3 GetRight() const;
	const D3DXVECTOR3 GetUp() const;
	const D3DXVECTOR3 GetForward() const;

	//====================================================
	//  [Transform]
	//====================================================
	void UpdateTransform();
	bool HasParent() const { return parent ? true : false; }

	void SetParent(TransformComponent* const new_parent);

	bool HasChilds() const { return childs.empty() != true; }

	const std::vector<TransformComponent*>& GetChilds() const { return childs; }
	TransformComponent* const GetChildFromIndex(const uint& index);

	uint GetChildCount() const { return static_cast<uint>(childs.size()); }

	void AddChild(TransformComponent* const child);
	void DeleteChild(TransformComponent* const child);

private:
	D3DXVECTOR3 local_scale = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 local_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 local_rotation = D3DXVECTOR3(0, 0, 0);

	D3DXMATRIX local;	// S * R * T
	D3DXMATRIX world;	// local * parent

	TransformComponent* parent = nullptr;
	std::vector<TransformComponent*> childs;
};
