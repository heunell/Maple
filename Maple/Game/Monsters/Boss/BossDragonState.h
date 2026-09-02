#pragma once

#include "Game/Monsters/MonsterState.h"
#include "BossPatternData.h"

class BossDragonState : public MonsterState
{
public:
	BossDragonState() = default;
	virtual ~BossDragonState() = default;
	BossDragonState(const BossDragonState&) = delete;
	BossDragonState(BossDragonState&&) = delete;
	BossDragonState& operator=(const BossDragonState&) = delete;
	BossDragonState& operator=(BossDragonState&&) = delete;

private:
	Weak<class MonsterComponent> _Owner;

	Weak<class MonsterState> _IdleState;

	Ptr<class BossPatternNotice> _Notice;

	Ptr<class BossDragon> _Dragon;

	FBossDragonPatternData _PatternData;

	bool _CastStarted = false;

	bool _DragonStarted = false;

private:
	void SpawnDragonPattern();

public:
	bool Init(Ptr<class MonsterComponent> Owner, Ptr<MonsterState> IdleState);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual bool CanSelect() const override;

	virtual void Destroy() override;
};