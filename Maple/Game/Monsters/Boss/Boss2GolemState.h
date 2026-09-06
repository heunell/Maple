#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "Boss2Golem.h"

class Boss2GolemState : public MonsterState
{
public:
	Boss2GolemState() = default;
	virtual ~Boss2GolemState() = default;
	Boss2GolemState(const Boss2GolemState&) = delete;
	Boss2GolemState(Boss2GolemState&&) = delete;
	Boss2GolemState& operator=(const Boss2GolemState&) = delete;
	Boss2GolemState& operator=(Boss2GolemState&&) = delete;

private:
	Weak<class Boss2Component> _Owner;

	ObjectPool<Boss2Golem> _GolemPool;

	FBoss2GolemPatternData _PatternData;

	bool _Spawned = false;

private:
	Ptr<Boss2Golem> CreateGolem();

	void EnableGolem(const Ptr<Boss2Golem>& Golem);

	void DisableGolem(const Ptr<Boss2Golem>& Golem);

	void SpawnGolemPattern();

public:
	bool Init(Ptr<class Boss2Component> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Destroy() override;

	void ReleaseGolem(Ptr<Boss2Golem> Golem);

	virtual void Reset();

};

