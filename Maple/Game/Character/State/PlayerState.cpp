#include "pch.h"
#include "PlayerState.h"

void PlayerState::Destroy() {}

Ptr<PlayerState> PlayerState::Tick(Ptr<class PlayerComponent> _PlayerComponent, float DeltaTime)
{
    return nullptr;
}

uint8 PlayerState::GetType() const
{
    return _StateType;
}