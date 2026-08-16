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

eCollisionState CollisionComponent::CheckState(std::pair<int32, int32>& DestKey)
{
    auto It = _ColliderReactions.find(DestKey);
    if (It == _ColliderReactions.end())
    {
        return It->second;
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

    return false;
}

void CollisionComponent::Tick(float DeltaTime)
{
    SceneComponent::Tick(DeltaTime);
}

void CollisionComponent::Collision(float DeltaTime)
{
    SceneComponent::Collision(DeltaTime);
}

bool CollisionComponent::Collision(Weak<CollisionComponent> Dest)
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
}

void CollisionComponent::SetCollisionProfile(const std::string& Name)
{
    Ptr<CollisionProfile> FoundVal = CollisionProfileManager::Instance().FindProfile(Name);
    if (nullptr == FoundVal)
    {
        return;
    }

    _Profile = FoundVal;
}

const Ptr<class CollisionProfile> CollisionComponent::GetProfile() const
{
    return Ptr<class CollisionProfile>();
}

void CollisionComponent::Invoke(eCollisionState, Weak<CollisionComponent> Dest, const std::pair<int32, int32>& DestKey)
{
}
