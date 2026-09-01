#pragma once
#include "Component/ActorComponent.h"

// 일반 몬스터 FSM 실행 기반
// 현재 State 보관 및 전환
// 골렘과 버섯도 사용 가능

class MonsterComponent : public ActorComponent
{
public:
	MonsterComponent() = default;
	virtual ~MonsterComponent() = default;
	MonsterComponent(const MonsterComponent&) = delete;
	MonsterComponent(MonsterComponent&&) = delete;
	MonsterComponent& operator=(const MonsterComponent&) = delete;
	MonsterComponent& operator=(MonsterComponent&&) = delete;

private:
	Ptr<class MonsterStateMachine> _StateMachine;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void TransitionState(Ptr<class MonsterState> State);

	Ptr<class MonsterBase> GetMonster() const;

	Ptr<class MonsterStateMachine> GetStateMachine() const;

	void SetBlackBoard(Ptr<class MonsterBlackBoard> BlackBoard);

	Ptr<class MonsterBlackBoard> GetBlackBoard() const;
};