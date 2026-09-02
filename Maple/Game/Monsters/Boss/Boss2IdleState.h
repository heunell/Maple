#pragma once
#include "Game/Monsters/MonsterState.h"

class Boss2IdleState : public MonsterState
{
public:
	Boss2IdleState() = default;
	virtual ~Boss2IdleState() = default;

	Boss2IdleState(const Boss2IdleState&) = delete;
	Boss2IdleState(Boss2IdleState&&) = delete;
	Boss2IdleState& operator=(const Boss2IdleState&) = delete;
	Boss2IdleState& operator=(Boss2IdleState&&) = delete;

private:
	float _ElapsedTime = 0.f;

public:
	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(
		Ptr<class MonsterComponent> Monster,
		float DeltaTime) override;
};