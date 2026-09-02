#pragma once

#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "BossMushroom.h"

class BossMushroomState : public MonsterState
{
public:
	BossMushroomState() = default;
	virtual ~BossMushroomState() = default;
	BossMushroomState(const BossMushroomState&) = delete;
	BossMushroomState(BossMushroomState&&) = delete;
	BossMushroomState& operator=(const BossMushroomState&) = delete;
	BossMushroomState& operator=(BossMushroomState&&) = delete;

private:
	Weak<class BossComponent> _Owner;

	ObjectPool<BossMushroom> _MushroomPool;

	FBossMushroomPatternData _PatternData;

	bool _Spawned = false;

private:
	Ptr<BossMushroom> CreateMushroom();

	void EnableMushroom(const Ptr<BossMushroom>& Mushroom);

	void DisableMushroom(const Ptr<BossMushroom>& Mushroom);

	void SpawnMushroomPattern();

public:
	bool Init(Ptr<class BossComponent> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual bool CanSelect() const override;

	virtual void Destroy() override;

	void ReleaseMushroom(Ptr<BossMushroom> Mushroom);
};