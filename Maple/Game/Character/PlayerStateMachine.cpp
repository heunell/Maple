#include "pch.h"
#include "PlayerStateMachine.h"
#include "PlayerState.h"

void PlayerStateMachine::Init(Ptr<class PlayerComponent> _Player, Ptr<class PlayerState> _State)
{
	_Owner = _Player;

	_CurrentState = _State;
}

void PlayerStateMachine::Destroy()
{
	DESTROY(_BlackBoard);
}

void PlayerStateMachine::Tick(float DeltaTime)
{
	_CurrentState->Tick(GetOwner(), DeltaTime);
}

void PlayerStateMachine::Transition(Ptr<class PlayerState> _State)
{
	Ptr<PlayerComponent> Owner = GetOwner();

	if(_CurrentState)
	{
		_CurrentState->Exit(Owner);
	}

	_CurrentState = _State;

	_CurrentState->Enter(Owner);
}

void PlayerStateMachine::HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
	Ptr<PlayerState> NewState = _CurrentState->HandleInput(GetOwner(), Action, ButtonEvent);

	if(nullptr == NewState)
	{
		return;
	}

	Transition(NewState);
}

Ptr<class PlayerComponent> PlayerStateMachine::GetOwner() const
{
	return Lock(_Owner);
}

Ptr<class PlayerState> PlayerStateMachine::GetCurrentState() const
{
	return _CurrentState;
}

uint8 PlayerStateMachine::GetCurrentStateType() const
{
	return _CurrentState->GetType();
}

void PlayerStateMachine::SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard)
{
	_BlackBoard = BlackBoard;
}
