#include "pch.h"
#include "PlayerIdleState.h"
#include "Component/PlayerComponent.h"
#include "Game/Character/Player.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"

Ptr<PlayerState> PlayerIdleState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action || ButtonEvent != INPUT_TYPE::HOLD)
    {
        return nullptr;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    if (Action->GetName() == "MOVE_RIGHT")
    {
        Player->MoveRight();

        return New<PlayerWalkState>();
    }

    if (Action->GetName() == "MOVE_LEFT")
    {
        Player->MoveLeft();

        return New<PlayerWalkState>();
    }

    if (Action->GetName() == "MOVE_JUMP" && Player->Jump())
    {
        return New<PlayerJumpState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerIdleState::GetAnimationType() const
{
    return ePlayerAnimationType::Idle;
}
