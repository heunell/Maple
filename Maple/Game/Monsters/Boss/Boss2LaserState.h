#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "Boss2Laser.h"

class Boss2LaserState : public MonsterState
{
public:
	Boss2LaserState() = default;
	virtual ~Boss2LaserState() = default;
	Boss2LaserState(const Boss2LaserState&) = delete;
	Boss2LaserState(Boss2LaserState&&) = delete;
	Boss2LaserState& operator=(const Boss2LaserState&) = delete;
	Boss2LaserState& operator=(Boss2LaserState&&) = delete;

private:
	Weak<class MonsterComponent> _Owner;

	Weak<class MonsterState> _IdleState;

	ObjectPool<Boss2Laser> _LaserPool;

	FBoss2LaserPatternData _PatternData;

	FVector3D _StartPosition = FVector3D::Zero;

	float _ElapsedTime = 0.f;

	int32 _SpawnCount = 0;

	bool _ActionStarted = false;

private:
	Ptr<Boss2Laser> CreateLaser();

	void EnableLaser(const Ptr<Boss2Laser>& Laser);

	void DisableLaser(const Ptr<Boss2Laser>& Laser);

	void SpawnLaserPattern();

public:
	bool Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Exit(Ptr<class MonsterComponent> Monster) override;

	virtual bool CanSelect() const override;

	virtual void Destroy() override;

	void ReleaseLaser(Ptr<Boss2Laser> Laser);
};