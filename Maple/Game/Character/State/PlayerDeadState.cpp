#include "pch.h"
#include "PlayerDeadState.h"

Ptr<PlayerState> PlayerDeadState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
	return Ptr<PlayerState>();
}

ePlayerAnimationType PlayerDeadState::GetAnimationType() const
{
	return ePlayerAnimationType();
}
