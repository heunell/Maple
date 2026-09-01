#pragma once
#include "Game/Monsters/MonsterState.h"

class BossIdleState : public MonsterState
{
public:
	BossIdleState() = default;
	virtual ~BossIdleState() = default;
	BossIdleState(const BossIdleState&) = delete;
	BossIdleState(BossIdleState&&) = delete;
	BossIdleState& operator=(const BossIdleState&) = delete;
	BossIdleState& operator=(BossIdleState&&) = delete;

private:
    float _ElapsedTime = 0.f;

public:
	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

    virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;
};