#include "pch.h"
#include "PlayerComponent.h"
#include "Game/Character/Player.h"
#include "Game/Character/PlayerStateMachine.h"

bool PlayerComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    ActorComponent::Init(Id, Name, Owner);

    _StateMachine = New<PlayerStateMachine>();

    _StateMachine->Init(This<PlayerComponent>(), nullptr);

    return true;
}

void PlayerComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    _StateMachine->Tick(DeltaTime);
}

void PlayerComponent::Destroy()
{
    ActorComponent::Destroy();

    DESTROY(_StateMachine);
}

void PlayerComponent::Transition(Ptr<class PlayerState> State)
{
    _StateMachine->Transition(State);
}

void PlayerComponent::HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    _StateMachine->HandleInput(Action, ButtonEvent);
}

Ptr<class Player> PlayerComponent::GetPlayer() const
{
    return Cast<Actor, Player>(GetOwner());
}

Ptr<class PlayerStateMachine> PlayerComponent::GetStateMachine() const
{
    return _StateMachine;
}

uint8 PlayerComponent::GetStateType() const
{
    return _StateMachine->GetCurrentStateType();
}

void PlayerComponent::SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard)
{
    _StateMachine->SetBlackBoard(BlackBoard);
}
