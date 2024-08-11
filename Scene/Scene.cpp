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
    // 이렇게 작성하면 함수를 호출할 때마다 Renderer를 호출할 필요가 없음.
    // 메모리를 희생해서 성능 향상
    renderer = context->GetSubsystem<Renderer>();

    //================================================================================================
    //   [Animaton]
    //================================================================================================
    // 최초 실행 시, Idle.xml에 정보가 저장되었기 때문에 최초 이후부터는 없어도 기동이 됨
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
    player->AddComponent<MoveScriptComponent>();        // 이동 스크립트
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
	//std::vector<const std::shared_ptr<class Actor>> actors;       // class Actor가 가지고 있는 주소값이 변경되지 않는 것. 주소가 가지고 있는 데이터는 변경될 수 있음 
    for (const auto& actor : actors)
        actor->Update();

    if (is_update == true)
    {
        renderer->UpdateRenderables(this);
        is_update = false;      // 한번만 업데이트해주기 위함
    }

}

const std::shared_ptr<class Actor> Scene::CreateActor(const bool& is_active)      // 복사 생성자(L-value Reference)
{
    std::shared_ptr<class Actor> actor = std::make_shared<Actor>(context);

    actor->SetActive(is_active);

    AddActor(actor);

    // 리턴을 받아야 외부에서 컴포넌트들을 붙여줄 수 있음
    return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)       // shared_ptr의 원본이 넘어감
{
    actors.emplace_back(actor);
    is_update = true;       // actor가 추가될 때 마다 업데이트됨. 대신 actor를 추가할 때마다 백터를 전부 갱신해버림. renderables에 있는 데이터들을 전부 갱신해야 함
}

//DX Direct x
// - GPU을 다루는 도구의 집합

// 디바이스 
// 디바이스 컨텍스트 - Rendering pipeline
// IA - VS - RS - PS - OM