#include "pch.h"
#include "SphereCollisionComponent.h"
#include "AABBCollisionComponent.h"
#include "OBBCollisionComponent.h"
#include "Collision/CollisionSystem.h"
#include "Core/AssetManager.h"
#include "Core/MeshManager.h"
#include "Core/Mesh.h"
#include "Shader/TransformCBuffer.h"

SphereCollisionComponent::SphereCollisionComponent()
{
	_Shape = eCollisionShape::Sphere;
}

SphereCollisionComponent::~SphereCollisionComponent()
{}

float SphereCollisionComponent::GetRadius() const
{
	return _Radius;
}

void SphereCollisionComponent::SetRadius(float Radius)
{
	_Radius = Radius;
}

bool SphereCollisionComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
	CollisionComponent::Init(Id, Name, Owner);

#if _DEBUG

	_Mesh = MESH_MANAGER->FindMesh("FrameSphere");

#endif

	_Type = COMPONENT_TYPE::SPHERE;

	return true;

}

void SphereCollisionComponent::Tick(float DeltaTime)
{
	CollisionComponent::Tick(DeltaTime);

	_Min._x = _World._position._x - _Radius;
	
	_Max._x = _World._position._x + _Radius;
	
	_Min._y = _World._position._y - _Radius;
	
	_Max._y = _World._position._y + _Radius;

	SetWorldScale(_Radius * 2.f, _Radius * 2.f, 1.f);
}

void SphereCollisionComponent::Collision(float DeltaTime)
{
	CollisionComponent::Collision(DeltaTime);
}

bool SphereCollisionComponent::Collision(Weak<CollisionComponent> Destination)
{
	Ptr<CollisionComponent> DestinationCollision = Lock<CollisionComponent>(Destination);
	
	if (nullptr == DestinationCollision)
	{
		return false;
	}

	switch (DestinationCollision->GetShape())
	{
	case eCollisionShape::Sphere:
		return CollisionSystem::SphereToSphere(This<SphereCollisionComponent>(), Cast<CollisionComponent, SphereCollisionComponent>(DestinationCollision));

	case eCollisionShape::AABB:
		return CollisionSystem::AABBToSphere(Cast<CollisionComponent, AABBCollisionComponent>(DestinationCollision), This<SphereCollisionComponent>());

	case eCollisionShape::OBB:
		return CollisionSystem::SphereToOBB(This<SphereCollisionComponent>(), Cast<CollisionComponent, OBBCollisionComponent>(DestinationCollision));

	default:
		break;
	}

	return false;
}

void SphereCollisionComponent::Render(float DeltaTime)
{
	CollisionComponent::Render(DeltaTime);
}

void SphereCollisionComponent::Destroy()
{
	CollisionComponent::Destroy();
}
