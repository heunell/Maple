#pragma once
#include "Game/Monsters/MonsterState.h"
#include "BossPatternData.h"

class Boss2MoveState : public MonsterState
{
private:
	Weak<class MonsterComponent> _Owner;

	Weak<class MonsterState> _IdleState;

	FBoss2MovePatternData _PatternData;

	FVector3D _TargetPosition = FVector3D::Zero;

	bool _TargetSelected = false;

public:
	Boss2MoveState() = default;
	virtual ~Boss2MoveState() = default;
	Boss2MoveState(const Boss2MoveState&) = delete;
	Boss2MoveState(Boss2MoveState&&) = delete;
	Boss2MoveState& operator=(const Boss2MoveState&) = delete;
	Boss2MoveState& operator=(Boss2MoveState&&) = delete;

public:
	bool Init(Ptr<class MonsterComponent> Owner, Ptr<class MonsterState> IdleState);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	virtual void Destroy() override;

private:
	bool SelectTargetPlatform(Ptr<class MonsterComponent> Monster);
};