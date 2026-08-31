#pragma once
#include "Object/Actor.h"

class BossGate : public Actor
{
public:
	BossGate() = default;
	virtual ~BossGate() = default;
	BossGate(const BossGate&) = delete;
	BossGate(BossGate&&) = delete;
	BossGate& operator=(const BossGate&) = delete;
	BossGate& operator=(BossGate&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Ptr<class AABBCollisionComponent> _Trigger;

	std::string _TargetMap;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Destroy() override;

	void SetTargetMap(const std::string& TargetMap);

	bool Interact();

private:
	void OnPlayerOverlap(Weak<class CollisionComponent> Collision);

	void OnPlayerRelease(Weak<class CollisionComponent> Collision);
};