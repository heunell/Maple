#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "BossBlade.h"

class BossBladeState : public MonsterState
{
public:
	BossBladeState() = default;
	virtual ~BossBladeState() = default;
	BossBladeState(const BossBladeState&) = delete;
	BossBladeState(BossBladeState&&) = delete;
	BossBladeState& operator=(const BossBladeState&) = delete;
	BossBladeState& operator=(BossBladeState&&) = delete;

private:
	Weak<class BossComponent> _Owner;

	ObjectPool<BossBlade> _BladePool;

	FBossBladePatternData _PatternData;

	bool _Spawned = false;

private:
	Ptr<BossBlade> CreateBlade();

	void EnableBlade(const Ptr<BossBlade>& Blade);

	void DisableBlade(const Ptr<BossBlade>& Blade);

	void SpawnBladePattern();

public:
	bool Init(Ptr<class BossComponent> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Destroy() override;

	void ReleaseBlade(Ptr<BossBlade> Blade);
};