#pragma once
#include "Core/Object.h"


class MonsterState : public Object
{
public:
	MonsterState() = default;
	virtual ~MonsterState() = default;
	MonsterState(const MonsterState&) = delete;
	MonsterState(MonsterState&&) = delete;
	MonsterState& operator=(const MonsterState&) = delete;
	MonsterState& operator=(MonsterState&&) = delete;

protected:
	uint8 _StateType = static_cast<uint8>(-1);

public:
	virtual void Enter(Ptr<class MonsterComponent> Monster);

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime);

	virtual void Exit(Ptr<class MonsterComponent> Monster);

	virtual void Destroy() override;

	uint8 GetType() const;
};