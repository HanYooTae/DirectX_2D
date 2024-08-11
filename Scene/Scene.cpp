// Execute.cpp
#include "stdafx.h"
#include "Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveScriptComponent.h"
#include "Scene/Component/AIScriptComponent.h"
#include "Scene/Component/TraceComponent.h"
#include "Scene/Component/AnimatorComponent.h"
#include "Scene/Component/TestComponent.h"

// TinyXML2

Scene::Scene(Context* context)
    :context(context)
{
    // �̷��� �ۼ��ϸ� �Լ��� ȣ���� ������ Renderer�� ȣ���� �ʿ䰡 ����.
    // �޸𸮸� ����ؼ� ���� ���
    renderer = context->GetSubsystem<Renderer>();

    //================================================================================================
    //   [Animaton]
    //================================================================================================
    // ���� ���� ��, Idle.xml�� ������ ����Ǿ��� ������ ���� ���ĺ��ʹ� ��� �⵿�� ��
    /*std::shared_ptr<Animation> idle_animation = std::make_shared<Animation>(context);
    idle_animation->AddKeyframe(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(30.0f, 38.0f), 200);
    idle_animation->AddKeyframe(D3DXVECTOR2(35.0f, 0.0f), D3DXVECTOR2(30.0f, 38.0f), 200);
    idle_animation->AddKeyframe(D3DXVECTOR2(70.0f, 0.0f), D3DXVECTOR2(30.0f, 38.0f), 200);
    idle_animation->SetRepeatType(RepeatType::Loop);
    idle_animation->SetSpriteTexture("Assets/Texture/metalslug.png");
    idle_animation->SetSpriteTextureSize(D3DXVECTOR2(400.0f, 600.0f));
    idle_animation->SetAnimationName("Idle");
    idle_animation->SaveToFile("Assets/Animation/Idle.xml");*/

    //================================================================================================
    //   [Actors]
    //================================================================================================
    std::shared_ptr<Actor> camera = CreateActor();
    camera->AddComponent<CameraComponent>();
    camera->SetName("MainCamera");

    std::shared_ptr<Actor> player = CreateActor();
    player->AddComponent<MeshRendererComponent>();
    player->AddComponent<MoveScriptComponent>();        // �̵� ��ũ��Ʈ
    player->AddComponent<AnimatorComponent>();
    player->AddComponent<TestComponent>();      // imgui

    player->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(100.0f, 100.0f, 1.0f));
    player->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(100.0f, 0.0f, 0.0f));

    auto animator = player->GetComponent<AnimatorComponent>();
    //animator->AddAnimation("Idle", idle_animation);
    animator->AddAnimation("Assets/Animation/Idle.xml");
    animator->SetAnimationMode(AnimationMode::Play);
    animator->SetCurrentAnimation("Idle");

    player->SetName("Player");

    std::shared_ptr<Actor> monster = CreateActor();
    monster->AddComponent<MeshRendererComponent>();
    monster->AddComponent<AIScriptComponent>();
    //monster->AddComponent<TraceComponent>();

    //monster->GetComponent<TraceComponent>()->SetTarget(player);
    monster->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3(100.0f, 100.0f, 1.0f));
    monster->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));
    monster->SetName("Monster");
}

Scene::~Scene()
{
}

void Scene::Update()
{
	//std::vector<const std::shared_ptr<class Actor>> actors;       // class Actor�� ������ �ִ� �ּҰ��� ������� �ʴ� ��. �ּҰ� ������ �ִ� �����ʹ� ����� �� ���� 
    for (const auto& actor : actors)
        actor->Update();

    if (is_update == true)
    {
        renderer->UpdateRenderables(this);
        is_update = false;      // �ѹ��� ������Ʈ���ֱ� ����
    }

}

const std::shared_ptr<class Actor> Scene::CreateActor(const bool& is_active)      // ���� ������(L-value Reference)
{
    std::shared_ptr<class Actor> actor = std::make_shared<Actor>(context);

    actor->SetActive(is_active);

    AddActor(actor);

    // ������ �޾ƾ� �ܺο��� ������Ʈ���� �ٿ��� �� ����
    return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)       // shared_ptr�� ������ �Ѿ
{
    actors.emplace_back(actor);
    is_update = true;       // actor�� �߰��� �� ���� ������Ʈ��. ��� actor�� �߰��� ������ ���͸� ���� �����ع���. renderables�� �ִ� �����͵��� ���� �����ؾ� ��
}

//DX Direct x
// - GPU�� �ٷ�� ������ ����

// ����̽� 
// ����̽� ���ؽ�Ʈ - Rendering pipeline
// IA - VS - RS - PS - OM