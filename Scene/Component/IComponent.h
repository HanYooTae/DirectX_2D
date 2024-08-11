#pragma once

enum class ComponentType : uint
{
	Unknown,
	Camera,
	Transform,
	MeshRenderer,
	MoveScript,
	AIScript,
	Trace,
	Animator,
	Test
};

class IComponent
{
public:
	template<typename T>
	static constexpr ComponentType DeduceComponentType();

public:
	IComponent
	(
		class Context* const context,		// context의 주소를 바꿔줄 경우는 없기 때문에 const 지정
		class Actor* const actor,
		class TransformComponent* const transform
	);
	virtual ~IComponent() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

	class Actor* GetActor() const { return actor; }
	void SetActor(class Actor* const actor) { this->actor = actor; }

	class TransformComponent* GetTransform() const { return transform; }
	void SetTransform(class TransformComponent* const transform) { this->transform = transform; }

	ComponentType GetComponentType() const { return component_type; }
	void SetComponentType(const ComponentType& type) { this->component_type = type; }

	bool IsEnabled() const { return is_enabled; }
	void SetEnabled(const bool& is_enabled) { this->is_enabled = is_enabled; }

protected:
	// 미리 컴파일된 헤더에 Context.h를 추가해줘서 전방선언을 안해도 되지만, 전방선언을 안할 경우, 헤더가 추가된 순서에 따라 인식을 하지 못할 경우가 있을 수 있기 때문에 전방선언을 해주는게 안전함.
	class Context* context = nullptr;

	class Actor* actor = nullptr;
	class TransformComponent* transform = nullptr;

	bool is_enabled = true;

	ComponentType component_type = ComponentType::Unknown;
};


// 그대로 헤더에 인라인을 두면 헤더파일에 있는 모든 타입에 대해서 UNKNOWN을 받기 때문에 특정 타입만 받게 하기 위하여 .cpp로 옮김
//template<typename T>
//inline constexpr ComponentType IComponent::DeduceComponentType()
//{
//	return ComponentType::Unknown();
//}