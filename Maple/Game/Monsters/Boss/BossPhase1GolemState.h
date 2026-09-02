#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "BossPhase1Golem.h"

class BossPhase1GolemState : public MonsterState
{
public:
	BossPhase1GolemState() = default;
	virtual ~BossPhase1GolemState() = default;
	BossPhase1GolemState(const BossPhase1GolemState&) = delete;
	BossPhase1GolemState(BossPhase1GolemState&&) = delete;
	BossPhase1GolemState& operator=(const BossPhase1GolemState&) = delete;
	BossPhase1GolemState& operator=(BossPhase1GolemState&&) = delete;

private:
	Weak<class BossComponent> _Owner;

	ObjectPool<BossPhase1Golem> _GolemPool;

	FBossPhase1GolemPatternData _PatternData;

	bool _Spawned = false;

private:
	Ptr<BossPhase1Golem> CreateGolem();

	void EnableGolem(const Ptr<BossPhase1Golem>& Golem);

	void DisableGolem(const Ptr<BossPhase1Golem>& Golem);

	void SpawnGolemPattern();

public:
	bool Init(Ptr<class BossComponent> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual bool CanSelect() const override;

	virtual void Destroy() override;

	void ReleaseGolem(Ptr<BossPhase1Golem> Golem);
};