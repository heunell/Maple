#pragma once
#include "Object/Actor.h"

class Skill : public Actor
{
public:
	Skill() = default;
	virtual ~Skill() = default;

protected:
	Weak<class Actor> _Owner;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner);

	virtual void Collision(float DeltaTime) = 0;

	virtual void Start() = 0;

	virtual void Update(float DeltaTime) = 0;

	virtual void End() = 0;

	virtual void Destroy() override;
};

