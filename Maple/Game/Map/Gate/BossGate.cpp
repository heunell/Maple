#include "pch.h"
#include "BossGate.h"
#include "GateInteractComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "World/GameLevel.h"
#include "World/MapManager.h"

bool BossGate::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    Ptr<Animation2DData> GetData = ANIMATION_MANAGER->FindAnimation("LucidBossGate");

    if (!GetData)
    {
        if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Gate\\Gate.json"), "Animations"))
        {
            return false;
        }

        GetData = ANIMATION_MANAGER->FindAnimation("LucidBossGate");
    }

    if (!GetData)
    {
        return false;
    }

    _Sprite = CreateSceneComponent<SpriteComponent>("BossGateSprite");

    if (!_Sprite)
    {
        return false;
    }

    _Sprite->SetRenderLayerName("Default");

    _Sprite->AddAnimationSequence(GetData, true);

    _Sprite->AttachToComponent(GetRoot());

    _Trigger = CreateSceneComponent<AABBCollisionComponent>("BossGateTrigger");

    if (!_Trigger)
    {
        return false;
    }

    _Trigger->SetBoxSize(152.f, 196.f);
    
    _Trigger->SetRelativePosition(0.f, 49.f, 0.f);

    _Trigger->AttachToComponent(GetRoot());

    _Trigger->SetCollisionProfile("Portal");

    _Trigger->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &BossGate::OnPlayerOverlap);

    _Trigger->SetCollisionCallBack(COLLISION_STATE_RELEASE, this, &BossGate::OnPlayerRelease);

    return true;
}

void BossGate::Destroy()
{
    _TargetMap.clear();

    _Trigger = nullptr;

    _Sprite = nullptr;

    Actor::Destroy();
}

void BossGate::SetTargetMap(const std::string& TargetMap)
{
    _TargetMap = TargetMap;
}

bool BossGate::Interact()
{
    if (!IsEnable() || _TargetMap.empty())
    {
        return false;
    }

    Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel());

    if (!CurrentLevel)
    {
        return false;
    }

    Ptr<MapManager> Manager = CurrentLevel->GetMapManager();

    if (!Manager)
    {
        return false;
    }

    if (!Manager->ChangeMap(_TargetMap))
    {
        return false;
    }

    return true;
}

void BossGate::OnPlayerOverlap(Weak<class CollisionComponent> Collision)
{
    Ptr<CollisionComponent> PlayerCollision = Lock(Collision);

    if (!PlayerCollision || PlayerCollision->GetName() != "AABB")
    {
        return;
    }

    Ptr<Actor> Player = PlayerCollision->GetOwner();

    if (!Player || !Player->IsTag("Player"))
    {
        return;
    }

    Ptr<GateInteractComponent> GateInteract = Player->FindActorComponent<GateInteractComponent>("GateInteract");

    if (!GateInteract)
    {
        return;
    }

    GateInteract->SetOverlappingGate(This<BossGate>());
}

void BossGate::OnPlayerRelease(Weak<class CollisionComponent> Collision)
{
    Ptr<CollisionComponent> PlayerCollision = Lock(Collision);

    if (!PlayerCollision || PlayerCollision->GetName() != "AABB")
    {
        return;
    }

    Ptr<Actor> Player = PlayerCollision->GetOwner();

    if (!Player || !Player->IsTag("Player"))
    {
        return;
    }

    Ptr<GateInteractComponent> GateInteract = Player->FindActorComponent<GateInteractComponent>("GateInteract");

    if (!GateInteract)
    {
        return;
    }

    GateInteract->ClearOverlappingGate(This<BossGate>());
}