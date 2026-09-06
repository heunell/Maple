#pragma once
#include "Game/Monsters/MonsterBase.h"
#include "BossPatternData.h"

class Boss2Golem : public MonsterBase
{
public:
	Boss2Golem() = default;
	virtual ~Boss2Golem() = default;
	Boss2Golem(const Boss2Golem&) = delete;
	Boss2Golem(Boss2Golem&&) = delete;
	Boss2Golem& operator=(const Boss2Golem&) = delete;
	Boss2Golem& operator=(Boss2Golem&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Ptr<class AABBCollisionComponent> _Collision;

	Weak<class Boss2GolemState> _Owner;

	Weak<class LucidPhase2> _Map;

	FBoss2GolemPatternData _PatternData;

	FVector3D _LandingPosition = FVector3D::Zero;

	int32 _PlatformIndex = -1;

	float _ElapsedTime = 0.f;

	bool _Summoning = false;

	bool _Falling = false;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class Boss2GolemState> Owner, Ptr<class LucidPhase2> Map, int32 PlatformIndex, const FVector3D& LandingPosition);

	void SetPoolEnable(bool Enable);
};