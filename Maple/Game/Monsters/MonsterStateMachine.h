#pragma once
#include "Core/Object.h"
#include <vector>

class MonsterStateMachine : public Object
{
public:
	MonsterStateMachine() = default;
	virtual ~MonsterStateMachine() = default;
	MonsterStateMachine(const MonsterStateMachine&) = delete;
	MonsterStateMachine(MonsterStateMachine&&) = delete;
	MonsterStateMachine& operator=(const MonsterStateMachine&) = delete;
	MonsterStateMachine& operator=(MonsterStateMachine&&) = delete;

private:
	Weak<class MonsterComponent> _Owner;

	Ptr<class MonsterState> _CurrentState;

	Ptr<class MonsterBlackBoard> _BlackBoard;

	std::vector<Ptr<class MonsterState>> _States;

public:
	bool Init(Ptr<class MonsterComponent> Owner);

	void Tick(float DeltaTime);

	bool AddState(Ptr<class MonsterState> State);

	void Transition(Ptr<class MonsterState> State);

	void SetBlackBoard(Ptr<class MonsterBlackBoard> BlackBoard);

	Ptr<class MonsterComponent> GetOwner() const;

	Ptr<class MonsterState> GetCurrentState() const;

	Ptr<class MonsterBlackBoard> GetBlackBoard() const;

	virtual void Destroy() override;
};