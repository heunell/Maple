#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "Boss2Laser.h"

#include <random>


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
	Weak<class Boss2Component> _Owner;

	Weak<class MonsterState> _IdleState;

	Weak<class Player> _Target;

	Ptr<class SpriteComponent> _ActionSprite;

	ObjectPool<Boss2Laser> _LaserPool;

	FBoss2LaserPatternData _PatternData;

	FBoss2LaserStateData _StateData;

	std::mt19937 _RandomEngine;

	FVector3D _ReturnPosition = FVector3D::Zero;

	float _SpawnElapsedTime = 0.f;

	float _NextSpawnTime = 0.f;

	float _ReturnElapsedTime = 0.f;

	int32 _SpawnedLaserCount = 0;

	bool _ActionStarted = false;

	bool _LaserStarted = false;


private:
	Ptr<Boss2Laser> CreateLaser();

	void EnableLaser(const Ptr<Boss2Laser>& Laser);

	void DisableLaser(const Ptr<Boss2Laser>& Laser);

	bool StartAction(Ptr<class MonsterComponent> Monster);

	bool SpawnLaser();

	void ReturnLucid(Ptr<class MonsterComponent> Monster);


public:
	bool Init(Ptr<class Boss2Component> Owner, Ptr<class MonsterState> IdleState, const FBoss2LaserStateData& StateData);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual bool CanSelect() const override;

	virtual void Exit(Ptr<class MonsterComponent> Monster) override;

	virtual void Destroy() override;

	void ReleaseLaser(Ptr<Boss2Laser> Laser);
};