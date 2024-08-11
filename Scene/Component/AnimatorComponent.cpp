#include "stdafx.h"
#include "AnimatorComponent.h"

AnimatorComponent::AnimatorComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
	timer = context->GetSubsystem<Timer>();
}

AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::Initialize()
{
}

void AnimatorComponent::Update()
{
	// 애니매이션을 돌려주면 안될 경우
	if (current_animation.expired() == true || IsPlaying() == false) return;

	frame_counter += timer->GetDeltaTimeMS();

	// 지나간 시간 >= 보여줘야할 시간 -> 다음 장면으로 넘겨줌
	if (frame_counter >= GetCurrentKeyframe()->time)
	{
		current_frame_number++;

		switch (current_animation.lock()->GetRepeatType())
		{
		case RepeatType::Once:
		{
			if (current_frame_number >= current_animation.lock()->GetKeyframeCount())
			{
				current_frame_number = current_animation.lock()->GetKeyframeCount() - 1;
				Pause();
			}
			break;
		}
		case RepeatType::Loop:
		{
			current_frame_number %= current_animation.lock()->GetKeyframeCount();
			break;
		}
		}

		frame_counter = 0.0f;
	}
}

void AnimatorComponent::Destroy()
{
}

const std::shared_ptr<class Animation> AnimatorComponent::GetCurrentAnimation() const
{
	return current_animation.expired() ? nullptr : current_animation.lock();
}

void AnimatorComponent::SetCurrentAnimation(const std::string& animation_name)
{
	if (animations.find(animation_name) == animations.end())
	{
		assert(false);
		return;
	}
	current_animation = animations[animation_name];

	current_frame_number = 0;
	frame_counter = 0.0f;

}

const Keyframe* const AnimatorComponent::GetCurrentKeyframe() const
{
	if (current_animation.expired() == true)
	{
		assert(false);
		return nullptr;
	}

	return current_animation.lock()->GetKeyframeFromIndex(current_frame_number);
}

void AnimatorComponent::AddAnimation(const std::string& animation_name, const std::shared_ptr<class Animation>& animation)
{
	if (animations.find(animation_name) != animations.end())
	{
		assert(false);
		return;
	}

	// animations 벡터에 저장
	animations[animation_name] = animation;
}

void AnimatorComponent::AddAnimation(const std::string& path)
{
	auto resource_manager = context->GetSubsystem<ResourceManager>();

	//auto animation = std::make_shared<Animation>(context);
	auto animation = resource_manager->Load<Animation>(path);

	//if (animation->LoadFromFile(path) == false)
	if (animation == nullptr)
	{
		assert(false);
		return;
	}

	AddAnimation(animation->GetResourceName(), animation);
}

void AnimatorComponent::Play()
{
	animation_mode = AnimationMode::Play;
	frame_counter = 0.0f;		// 초기화 값
}

void AnimatorComponent::Stop()
{
	animation_mode = AnimationMode::Stop;
	current_frame_number = 0;
	frame_counter = 0.0f;
}

void AnimatorComponent::Pause()
{
	// Update에서 처리해줄 예정
	animation_mode = AnimationMode::Pause;
}
