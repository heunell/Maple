#pragma once

#include "Game/Monsters/MonsterState.h"

class Boss2DragonState : public MonsterState
{
public:
	Boss2DragonState() = default;
	virtual ~Boss2DragonState() = default;
	Boss2DragonState(const Boss2DragonState&) = delete;
	Boss2DragonState(Boss2DragonState&&) = delete;
	Boss2DragonState& operator=(const Boss2DragonState&) = delete;
	Boss2DragonState& operator=(Boss2DragonState&&) = delete;

private:
	Weak<class MonsterComponent> _Owner;

	Weak<class MonsterState> _IdleState;

	Ptr<class Boss2Dragon> _Dragon;

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