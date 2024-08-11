#pragma once
#include "ISubsystem.h"

// �� ����
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
		�޸𸮰� ������ -> ���ҵ��� ���������� ó���ϱ� ȿ����
		�ǵ� ���Ҹ� �����ϰ� ������ ����, �����ϱ⿡ �Ҹ�
	*/
};

