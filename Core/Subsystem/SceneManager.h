#pragma once
#include "ISubsystem.h"

// 맵 관리
class SceneManager final : public ISubsystem
{
public:
	SceneManager(class Context* context);
	~SceneManager();

	virtual bool Initialize() override;
	virtual void Update() override;

	const std::map<std::string, std::shared_ptr<class Scene>>& GetAllScenes() const { return scenes; }

	class Scene* const GetCurrentScene();
	void SetCurrentScene(const std::string& scene_name);

	class Scene* const RegisterScene(const std::string& scene_name);

private:
	std::map<std::string, std::shared_ptr<class Scene>> scenes;
	std::weak_ptr<class Scene> current_scene;

	/*
		메모리가 연속적 -> 원소들을 순차적으로 처리하기 효율적
		맨뒤 원소를 제외하고 원소의 삽입, 삭제하기에 불리
	*/
};

