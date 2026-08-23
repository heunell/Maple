#pragma once
#include "Object/Actor.h"
#include "Component/AABBCollisionComponent.h"

class LucidEntry : public Actor
{
public:
	LucidEntry();
	virtual ~LucidEntry();
	LucidEntry(const LucidEntry&) = delete;
	LucidEntry(LucidEntry&&) = delete;
	LucidEntry& operator = (const LucidEntry&) = delete;
	LucidEntry& operator = (LucidEntry&&) = delete;

private:
	Ptr<AABBCollisionComponent> _Clamp;

	Ptr<AABBCollisionComponent> _LeftWall;

	Ptr<AABBCollisionComponent> _RightWall;
	
	Ptr<AABBCollisionComponent> _Floor;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;
};

