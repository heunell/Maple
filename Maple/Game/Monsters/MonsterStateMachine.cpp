#include "pch.h"
#include "MonsterStateMachine.h"
#include "MonsterComponent.h"
#include "MonsterState.h"
#include "MonsterBlackBoard.h"

bool MonsterStateMachine::Init(Ptr<MonsterComponent> Owner)
{
	if (!Owner)
	{
		return false;
	}

	_Owner = Owner;

	return true;
}

void MonsterStateMachine::Tick(float DeltaTime)
{
	Ptr<MonsterComponent> Owner = GetOwner();

	if (!Owner || !_CurrentState)
	{
		return;
	}

	Ptr<MonsterState> NextState = _CurrentState->Tick(Owner, DeltaTime);

	if (NextState)
	{
		Transition(NextState);
	}
}

bool MonsterStateMachine::AddState(Ptr<MonsterState> State)
{
	if (!State)
	{
		return false;
	}

	for (Ptr<MonsterState>& RegisteredState : _States)
	{
		if (RegisteredState == State)
		{
			return false;
		}
	}

	_States.push_back(State);

	return true;
}

void MonsterStateMachine::Transition(Ptr<MonsterState> State)
{
	if (!State || State == _CurrentState)
	{
		return;
	}

	Ptr<MonsterComponent> Owner = GetOwner();

	if (!Owner)
	{
		return;
	}

	if (_CurrentState)
	{
		_CurrentState->Exit(Owner);

		Delete(_CurrentState);
	}

	_CurrentState = State;

	_CurrentState->Enter(Owner);
}

void MonsterStateMachine::SetBlackBoard(Ptr<MonsterBlackBoard> BlackBoard)
{
	_BlackBoard = BlackBoard;
}

Ptr<MonsterComponent> MonsterStateMachine::GetOwner() const
{
	return Lock(_Owner);
}

Ptr<MonsterState> MonsterStateMachine::GetCurrentState() const
{
	return _CurrentState;
}

Ptr<MonsterBlackBoard> MonsterStateMachine::GetBlackBoard() const
{
	return _BlackBoard;
}

void MonsterStateMachine::Destroy()
{
	Ptr<MonsterComponent> Owner = GetOwner();

	Ptr<MonsterState> CurrentState = _CurrentState;

	if (CurrentState && Owner)
	{
		CurrentState->Exit(Owner);
	}

	Delete(_CurrentState);

	bool CurrentStateRegistered = false;

	for (Ptr<MonsterState>& RegisteredState : _States)
	{
		if (RegisteredState == CurrentState)
		{
			CurrentStateRegistered = true;

			break;
		}
	}

	if (CurrentState && !CurrentStateRegistered)
	{
		DESTROY(CurrentState);
	}

	for (Ptr<MonsterState>& State : _States)
	{
		DESTROY(State);
	}

	_States.clear();

	DESTROY(_BlackBoard);

	_Owner.reset();
}