#pragma once
#include "IComponent.h"

enum State
{
	Trace,
	Avoid
};

class TraceComponent : public IComponent
{
public:
	using IComponent::IComponent;
	~TraceComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetTarget(const std::shared_ptr<const class Actor> target)
	{
		// 데이터가 같다면 리턴
		if (actor == target.get()) return;

		// 아닐 경우, 같게 만들어줌
		this->target = target;
	}

private:
	// 주소는 수정되지만(weak_ptr) Actor는 변경안됨
	std::weak_ptr<const class Actor> target;

	Stopwatch timer;
	int speed = 40;

	enum State current_state = State::Trace;
};

