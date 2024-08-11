#pragma once
class Engine final
{
public:
	Engine();
	~Engine();

	void Update();
	void Render();

private:
	//std::shared_ptr<class SceneManager> scene_manager;		// scene_manager는 하나씩 등록해줬음.
	class Context* context = nullptr;
};

