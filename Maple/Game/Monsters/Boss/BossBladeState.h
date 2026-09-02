#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "BossBlade.h"

#include <string>

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
	Weak<class MonsterComponent> _Owner;

	Weak<class MonsterState>     _IdleState;
 							     
	ObjectPool<BossBlade>        _BladePool;
							     
	FBossBladePatternData        _PatternData;
							     
	FBossBladeStateData          _StateData;

	bool _Spawned = false;

private:
	Ptr<BossBlade> CreateBlade();

	void EnableBlade(const Ptr<BossBlade>& Flower);

	void DisableBlade(const Ptr<BossBlade>& Flower);

	void SpawnBladePattern();

public:
	bool Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState, const FBossBladeStateData& StateData);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Destroy() override;

	void ReleaseBlade(Ptr<BossBlade> Flower);
};