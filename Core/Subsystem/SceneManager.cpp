#include "stdafx.h"
#include "SceneManager.h"
#include "Scene/Scene.h"


SceneManager::SceneManager(Context* context)
    :ISubsystem(context)
{
}

SceneManager::~SceneManager()
{
    scenes.clear();
}

bool SceneManager::Initialize()
{
    RegisterScene("First");
    SetCurrentScene("First");

    return true;
}

void SceneManager::Update()
{
    if (current_scene.expired() != true)
        current_scene.lock()->Update();
}

Scene* const SceneManager::GetCurrentScene()
{
    return current_scene.expired() ? nullptr : current_scene.lock().get();
}

void SceneManager::SetCurrentScene(const std::string& scene_name)
{
    if (scenes.find(scene_name) == scenes.end())
    {
        assert(false);
        return;
    }

    current_scene = scenes[scene_name];
}

Scene* const SceneManager::RegisterScene(const std::string& scene_name)
{
    if (scenes.find(scene_name) != scenes.end())
    {
        assert(false);
        return nullptr;
    }

    std::shared_ptr<Scene> new_scene = std::make_shared<Scene>(context);

    scenes[scene_name] = new_scene;

    return new_scene.get();
}
