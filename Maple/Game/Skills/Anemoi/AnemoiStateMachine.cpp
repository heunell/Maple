#include "pch.h"
#include "AnemoiStateMachine.h"
#include "AnemoiActor.h"
#include "AnemoiState.h"
void AnemoiStateMachine::Init(Ptr<class AnemoiActor> Anemoi, Ptr<class AnemoiState> State)
{
	_Owner = Anemoi;

    Transition(State);
}

void AnemoiStateMachine::Tick(float DeltaTime)
{
	Ptr<AnemoiActor> Owner = GetOwner();

    if (!Owner || !_CurrentState)
    {
        return;
    }

    Ptr<AnemoiState> NextState = _CurrentState->Tick(Owner, DeltaTime);

    if (NextState)
    {
        Transition(NextState);
    }
}

void AnemoiStateMachine::Transition(Ptr<class AnemoiState> State)
{
	if (!State || State == _CurrentState)
    {
        return;
    }

    Ptr<AnemoiActor> Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    if (_CurrentState)
    {
        _CurrentState->Exit(Owner);

        DESTROY(_CurrentState);
    }

    _CurrentState = State;

    _CurrentState->Enter(Owner);
}

Ptr<class AnemoiActor> AnemoiStateMachine::GetOwner() const
{
	return Lock(_Owner);
}

Ptr<class AnemoiState> AnemoiStateMachine::GetCurrentState() const
{
	return _CurrentState;
}

void AnemoiStateMachine::Destroy()
{
	Ptr<AnemoiActor> Owner = GetOwner();

    if (_CurrentState)
    {
        _CurrentState->Exit(Owner);

        DESTROY(_CurrentState);
    }

    _Owner.reset();
}
