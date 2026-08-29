#include "pch.h"
#include "PlayerStateMachine.h"

#include "Component/PlayerComponent.h"
#include "PlayerState.h"

void PlayerStateMachine::Init(Ptr<PlayerComponent> Player, Ptr<PlayerState> MovementState, Ptr<PlayerState> ActionState)
{
    _Owner = Player;

    TransitionMovement(MovementState);

    TransitionAction(ActionState);
}

void PlayerStateMachine::Destroy()
{
    if (_CurrentMovementState)
    {
        _CurrentMovementState->Exit(GetOwner());

        DESTROY(_CurrentMovementState);
    }

    if (_CurrentActionState)
    {
        _CurrentActionState->Exit(GetOwner());

        DESTROY(_CurrentActionState);
    }

    DESTROY(_BlackBoard);
}

void PlayerStateMachine::Tick(float DeltaTime)
{
    Ptr<PlayerComponent> Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    if (_CurrentMovementState)
    {
        Ptr<PlayerState> MovementState = _CurrentMovementState->Tick(Owner, DeltaTime);

        if(MovementState)
        {
            TransitionMovement(MovementState);
        }
    }

    if (_CurrentActionState)
    {
        Ptr<PlayerState> ActionState = _CurrentActionState->Tick(Owner, DeltaTime);

        if(ActionState)
        {
            TransitionAction(ActionState);
        }
    }
}

void PlayerStateMachine::HandleInput(Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    Ptr<PlayerComponent> Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    if (_CurrentMovementState)
    {
        Ptr<PlayerState> MovementState = _CurrentMovementState->HandleInput(Owner, Action, ButtonEvent);

        if (MovementState)
        {
            TransitionMovement(MovementState);
        }
    }

    if (_CurrentActionState)
    {
        Ptr<PlayerState> ActionState = _CurrentActionState->HandleInput(Owner, Action, ButtonEvent);

        if (ActionState)
        {
            TransitionAction(ActionState);
        }
    }
}

void PlayerStateMachine::TransitionMovement(Ptr<PlayerState> State)
{
    if (!State || State == _CurrentMovementState)
    {
        return;
    }

    Ptr<PlayerComponent> Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    if (_CurrentMovementState)
    {
        _CurrentMovementState->Exit(Owner);

        DESTROY(_CurrentMovementState);
    }

    _CurrentMovementState = State;

    _CurrentMovementState->Enter(Owner);
}

void PlayerStateMachine::TransitionAction(Ptr<PlayerState> State)
{
    if (!State || State == _CurrentActionState)
    {
        return;
    }

    Ptr<PlayerComponent> Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    if (_CurrentActionState)
    {
        _CurrentActionState->Exit(Owner);

        DESTROY(_CurrentActionState);
    }

    _CurrentActionState = State;

    _CurrentActionState->Enter(Owner);
}

Ptr<PlayerComponent> PlayerStateMachine::GetOwner() const
{
    return Lock(_Owner);
}

Ptr<PlayerState> PlayerStateMachine::GetCurrentMovementState() const
{
    return _CurrentMovementState;
}

Ptr<PlayerState> PlayerStateMachine::GetCurrentActionState() const
{
    return _CurrentActionState;
}

uint8 PlayerStateMachine::GetCurrentMovementStateType() const
{
    if (!_CurrentMovementState)
    {
        return static_cast<uint8>(-1);
    }

    return _CurrentMovementState->GetType();
}

uint8 PlayerStateMachine::GetCurrentActionStateType() const
{
    if (!_CurrentActionState)
    {
        return static_cast<uint8>(-1);
    }

    return _CurrentActionState->GetType();
}

void PlayerStateMachine::SetBlackBoard(Ptr<PlayerBlackBoard> BlackBoard)
{
    _BlackBoard = BlackBoard;
}

