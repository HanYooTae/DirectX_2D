#include "stdafx.h"
#include "Actor.h"
#include "Component/MeshRendererComponent.h"
#include "Component/TransformComponent.h"

Actor::Actor(Context* const context)
    : context(context)
{
	AddComponent<TransformComponent>();
}

Actor::~Actor()
{
	components.clear();
	components.shrink_to_fit();		// »©µµ µÊ
}

void Actor::Initialize()
{
	for (auto& component : components)
		component->Initialize();
}

void Actor::Update()
{
    if (is_active == false) return;

	for (auto& component : components)
	{
		if (component->IsEnabled() == false) continue;

		component->Update();
	}


}

void Actor::Destroy()
{
	for (auto& component : components)
		component->Destroy();
}

bool Actor::HasComponent(const ComponentType& type)
{
	for (auto& component : components)
		if (component->GetComponentType() == type) return true;

	return false;
}
