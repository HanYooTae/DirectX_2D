#pragma once
#include"Component/IComponent.h"

// Component가 Actor의 세부사항들 관리
class Actor final
{
public:
	Actor(class Context* const context);
	~Actor();

	void Initialize();
	void Update();
	void Destroy();

	//=======================================================================
	//	Property(특성, 성질) - 멤버들의 getter, setter를 묶어서 부름
	//=======================================================================
	const std::string& GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	bool IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active; }

	const std::shared_ptr<class TransformComponent>& GetTransform() const { return transform; }

	class TransformComponent* GetTransform_Raw() const { return transform.get(); }

	//=======================================================================
	//	Component
	//=======================================================================
	template <typename T>
	const std::shared_ptr<T> AddComponent();	// 컴포넌트 추가

	template <typename T>
	const std::shared_ptr<T> GetComponent();	// 외부에서 컴포넌트 얻어옴

	template <typename T>
	T* GetComponent_Raw();

	template <typename T>
	const std::vector<std::shared_ptr<T>> GetComponents();

	const std::vector<std::shared_ptr<IComponent>>& GetAllComponents() const { return components; }

	bool HasComponent(const ComponentType& type);	// 해당 컴포넌트 타입이 있는지 찾아봄

	template <typename T>
	bool HasComponent();

	template <typename T>
	void RemoveComponent();		// 컴포넌트 삭제

private:
	class Context* context = nullptr;

	std::string name;
	bool is_active = true;
	std::shared_ptr<class TransformComponent> transform;	// Actor의 중심 위치를 정의(world 행렬)

	std::vector<std::shared_ptr<IComponent>> components;	// Actor를 구성하는 component
};

template<typename T>
inline const std::shared_ptr<T> Actor::AddComponent()
{
	// is_base_of<T1, T2> : T2가 T1을 상속받았을 시 value값이 true, 아니라면 false
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();

	// 위의 컴포넌트를 가지고 있을 경우 return하여 전에 만들어놨던 1개의 컴포넌트만 가질 수 있도록 한다.
	if (HasComponent(type) == true)
		return GetComponent<T>();

	components.emplace_back
	(
		std::make_shared<T>
		(
			context,
			this,
			transform.get()
		)
	);

	std::shared_ptr<T> new_component = std::static_pointer_cast<T>(components.back());
	new_component->Initialize();				// 컴포넌트 초기화
	new_component->SetComponentType(type);		// 컴포넌트 타입 세팅


	// if constexpr : 상수타임 때 보장이 되어야 함.  http://egloos.zum.com/sweeper/v/3203973
	//템플릿은 상수 시간에 체크되어야 함
	if constexpr (std::is_same<T, class TransformComponent>::value == true)
		transform = new_component;

	return new_component;
}

template<typename T>
inline const std::shared_ptr<T> Actor::GetComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();

	for (const auto& component : components)
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component);

	return nullptr;
}

template<typename T>
inline T* Actor::GetComponent_Raw()
{
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();

	for (const auto& component : components)
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component).get();

	return nullptr;
}

template<typename T>
inline const std::vector<std::shared_ptr<T>> Actor::GetComponents()
{
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();

	std::vector<std::shared_ptr> temp_components;

	for (const auto& component : components)
	{
		if (component->GetComponentType() != type) continue;

		temp_components.emplace_back(std::static_pointer_cast<T>(component));
	}

	return temp_components;
}

template<typename T>
inline bool Actor::HasComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	return HasComponent(IComponent::DeduceComponentType<T>());
}

template<typename T>
inline void Actor::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value == true, "Provider type does not implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();

	for (auto iter = components.begin(); iter != components.end();)
	{
		std::shared_ptr<T> component = *iter;

		if (component->GetComponentType() == type)
		{
			component->Destroy();
			component->reset();

			iter = components.erase(iter);
		}
		else
			iter++;
	}
}
