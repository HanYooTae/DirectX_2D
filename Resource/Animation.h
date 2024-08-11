#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
	Once,	// 동작반복X
	Loop	// 동작반복O
};

// 장면 하나하나를 만들어주는 것
struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
		: offset(offset)
		, size(size)
		, time(time)
	{}

	D3DXVECTOR2 offset{ 0.0f, 0.0f };	// offset == left.top
	D3DXVECTOR2 size{ 0.0f, 0.0f };		// size == height, width
	double time{ 0.0 };		// 다음 장면으로 넘어갈 때 걸리는 시간
							// 다음 장면으로 넘기려면 몇초가 걸리는지
};

class Animation final : public IResource
{
public:
	Animation(class Context* const context);
	~Animation();

	virtual bool SaveToFile(const std::string& path) override;
	// 애니매이션 데이터를 어떻게 저장할지
	virtual bool LoadFromFile(const std::string& path) override;

	const std::string& GetSpriteTexturePath() const { return sprite_texture_path; }
	void SetSpriteTexturePath(const std::string& path) { this->sprite_texture_path = path; }

	const RepeatType& GetRepeatType() const { return repeat_type; }
	void SetRepeatType(const RepeatType& repeat_type) { this->repeat_type = repeat_type; }

	const D3DXVECTOR2& GetSpriteTextureSize() const { return sprite_texture_size; }
	void SetSpriteTextureSize(const D3DXVECTOR2 size) { this->sprite_texture_size = size; }

	const std::shared_ptr<class D3D11_Texture>& GetSpriteTexture() const { return sprite_texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& sprite_texture) { this->sprite_texture = sprite_texture; }
	void SetSpriteTexture(const std::string& path);		//쉐이더 리소스가 안만들어져있을 경우, 경로를 보내주면 경로에 있는 텍스처를 만들어줌

	const std::vector<Keyframe>& GetKeyframes() const { return keyframes; }
	void SetKeyframes(const std::vector<Keyframe>& keyframes) { this->keyframes = keyframes; }

	void AddKeyframe(const Keyframe& keyframe);
	void AddKeyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double time);

	const Keyframe* const GetKeyframeFromIndex(const uint& index);
	const uint GetKeyframeCount() const { return keyframes.size(); }

private:
	RepeatType repeat_type = RepeatType::Loop;
	D3DXVECTOR2 sprite_texture_size = D3DXVECTOR2(1.0f, 1.0f);

	std::shared_ptr<class D3D11_Texture> sprite_texture;
	std::string sprite_texture_path = "";

	std::vector<Keyframe> keyframes;
};