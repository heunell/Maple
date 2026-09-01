#pragma once
#include "Object/Actor.h"

class BossBlade : public Actor
{
public:
	BossBlade() = default;
	virtual ~BossBlade() = default;
	BossBlade(const BossBlade&) = delete;
	BossBlade(BossBlade&&) = delete;
	BossBlade& operator=(const BossBlade&) = delete;
	BossBlade& operator=(BossBlade&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Weak<class BossBladeState> _Owner;

	FVector3D _Direction;

	float _ElapsedTime = 0.f;

	float _Speed = 60.f;

	float _LifeTime = 6.35f;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class BossBladeState> Owner, const FVector3D& Position, const FVector3D& Direction, int32 ResourceIndex);

	void SetPoolEnable(bool Enable);
};