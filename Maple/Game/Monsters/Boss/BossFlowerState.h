#pragma once

#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "BossFlower.h"

class BossFlowerState : public MonsterState
{
public:
	BossFlowerState() = default;
	virtual ~BossFlowerState() = default;
	BossFlowerState(const BossFlowerState&) = delete;
	BossFlowerState(BossFlowerState&&) = delete;
	BossFlowerState& operator=(const BossFlowerState&) = delete;
	BossFlowerState& operator=(BossFlowerState&&) = delete;

private:
	Weak<class BossComponent> _Owner;

	ObjectPool<BossFlower> _FlowerPool;

	FBossFlowerPatternData _PatternData;

	bool _Spawned = false;

private:
	Ptr<BossFlower> CreateFlower();

	void EnableFlower(const Ptr<BossFlower>& Flower);

	void DisableFlower(const Ptr<BossFlower>& Flower);

	void SpawnFlowerPattern();

public:
	bool Init(Ptr<class BossComponent> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Destroy() override;

	void ReleaseFlower(Ptr<BossFlower> Flower);

	virtual void Reset() override;
};