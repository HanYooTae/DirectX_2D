#include "stdafx.h"
#include "AIScriptComponent.h"
#include "TransformComponent.h"

void AIScriptComponent::Initialize()
{
}

void AIScriptComponent::Update()
{
	D3DXVECTOR3 position = transform->GetPosition();

	switch (direction)
	{
	case Direction::Up:     position.y++; break;
	case Direction::Right:  position.x++; break;
	case Direction::Down:   position.y--; break;
	case Direction::Left:   position.x--; break;
	}

	int width = Settings::Get().GetWidth();
	int height = Settings::Get().GetHeight();

	if (stopwatch.GetElapsedTimeMs() >= 3000.0)
	{
		Direction new_dir = static_cast<Direction>(Math::Random(0, 3));

		if (direction == new_dir)
		{
			int dir_num = static_cast<int>(new_dir);
			dir_num = (dir_num <= 1) ? dir_num + 2 : dir_num - 2;

			direction = static_cast<Direction>(dir_num);
		}
		else
			direction = new_dir;

		stopwatch.Start();
	}
	transform->SetPosition(position);
}

void AIScriptComponent::Destroy()
{
}
