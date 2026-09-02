#pragma once
#include "Object/Actor.h"

class BossFlower : public Actor
{
public:
	BossFlower() = default;
	virtual ~BossFlower() = default;
	BossFlower(const BossFlower&) = delete;
	BossFlower(BossFlower&&) = delete;
	BossFlower& operator=(const BossFlower&) = delete;
	BossFlower& operator=(BossFlower&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Weak<class BossFlowerState> _Owner;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class BossFlowerState> Owner, const FVector3D& Position, int32 ResourceIndex, float Rotation);

	void SetPoolEnable(bool Enable);
};