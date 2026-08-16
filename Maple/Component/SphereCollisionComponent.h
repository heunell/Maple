#pragma once
#include "CollisionComponent.h"

class SphereCollisionComponent : public CollisionComponent
{
public:
	SphereCollisionComponent();

	virtual ~SphereCollisionComponent();

private:
	float _Radius = 0.f;

public:
	float GetRadius() const;

	void SetRadius(float Radius);

	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual bool Collision(Weak<CollisionComponent> Destination);

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;
};

