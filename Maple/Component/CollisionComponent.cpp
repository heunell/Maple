#include "pch.h"
#include "CollisionComponent.h"
#include "Collision/CollisionProfileManager.h"
#include "Object/Actor.h"
#include "World/Level.h"
#include "Shader/ShaderManager.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/ColorCBuffer.h"
#include "Core/Mesh.h"

CollisionComponent::CollisionComponent()
{
#ifdef _DEBUG

    _IsRender = true;

    _RenderLayerName = "Collider";

#endif
}

CollisionComponent::~CollisionComponent()
{}

eCollisionState CollisionComponent::CheckState(std::pair<int32, int32>& DestnationKey)
{
    auto It = _ColliderReactions.find(DestnationKey);

    if (It == _ColliderReactions.end())
    {
        return eCollisionState::COLLISION_STATE_RELEASE;
    }
    return It->second;
}

const std::pair<int32, int32>& CollisionComponent::GetColliderID() const
{
    return _ColliderID;
}

bool CollisionComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    SceneComponent::Init(Id, Name, Owner);

    _ColliderID = std::make_pair(Owner->GetActorID(), GetComponentID());

    Ptr<Level> level = Lock<Level>(_Level);

    if (nullptr == level)
    {
        return false;
    }

    level->AddCollision(_ColliderID, This<CollisionComponent>());

#if _DEBUG

    _Shader = ShaderManager::Instance().FindShader("FrameMeshShader");

    _TransformCBuffer = ShaderManager::Instance().FindCBuffer<TransformCBuffer>("Transform");

    _ColorCBuffer = ShaderManager::Instance().FindCBuffer<ColorCBuffer>("Color");

#endif

    _Type = COMPONENT_TYPE::COLLISION;

    return true;
}

void CollisionComponent::Tick(float DeltaTime)
{
    SceneComponent::Tick(DeltaTime);
}

void CollisionComponent::Collision(float DeltaTime)
{
    SceneComponent::Collision(DeltaTime);
}

bool CollisionComponent::Collision(Weak<CollisionComponent> Destination)
{
    return false;
}

void CollisionComponent::Render(float DeltaTime)
{
    SceneComponent::Render(DeltaTime);

#if _DEBUG

    if (!_Mesh || !_Shader)
    {
        return;
    }

    _TransformCBuffer->SetWorldMatrix(_Matrix._World);

    Ptr<Level> level = Lock<Level>(_Level);

    if (nullptr == level)
    {
        return;
    }
    
    _TransformCBuffer->SetViewMatrix(level->GetViewMatrix());

    _TransformCBuffer->SetProjectionMatrix(level->GetProjectionMatrix());

    _TransformCBuffer->Update();

    if (_ColliderReactions.size() > 0)
    {
        _ColorCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        _ColorCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);
    }

    _ColorCBuffer->Update();

    _Shader->SetShader();

    _Mesh->Render();

#endif
}

void CollisionComponent::Destroy()
{
    SceneComponent::Destroy();

    Ptr<Level> level = Lock<Level>(_Level);

    if (nullptr == level)
    {
        return;
    }

    level->RemoveCollision(_ColliderID);

    for (auto& It : _ColliderReactions)
    {
        std::pair<int32, int32> Key = It.first;

        Ptr<CollisionComponent> FoundCollision = level->FindCollider(Key);

        if (nullptr == FoundCollision)
        {
            continue;
        }

        FoundCollision->_ColliderReactions.erase(_ColliderID);
    }
}

void CollisionComponent::SetCollisionProfile(const std::string& Name)
{
    Ptr<CollisionProfile> FoundValue = CollisionProfileManager::Instance().FindProfile(Name);

    if (nullptr == FoundValue)
    {
        return;
    }

    _Profile = FoundValue;
}

const Ptr<class CollisionProfile> CollisionComponent::GetProfile() const
{
    return _Profile;
}

void CollisionComponent::Invoke(eCollisionState State, Weak<CollisionComponent> Destination, const std::pair<int32, int32>& DestinationKey)
{
    Ptr<CollisionComponent> DestinationCollision = Lock<CollisionComponent>(Destination);

    if (nullptr == DestinationCollision)
    {
        return;
    }

    if (State == eCollisionState::COLLISION_STATE_BLOCK || State == eCollisionState::COLLISION_STATE_OVERLAP)
    {
        _ColliderReactions.emplace(std::make_pair(DestinationKey, State));
    }
    else
    {
        _ColliderReactions.erase(DestinationKey);
    }

    if (_CollisionCallBack[State])
    {
        _CollisionCallBack[State](Destination);
    }
}
