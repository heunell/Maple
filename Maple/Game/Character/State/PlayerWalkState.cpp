#include "pch.h"
#include "PlayerWalkState.h"
#include "Component/PlayerComponent.h"
#include "Game/Character/Player.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerProneState.h"

Ptr<PlayerState> PlayerWalkState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action)
    {
        return nullptr;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    if (ButtonEvent == INPUT_TYPE::HOLD)
    {
        if (Action->GetName() == "MOVE_DOWN")
        {
            return New<PlayerProneState>();
        }

        if (Action->GetName() == "MOVE_RIGHT")
        {
            Player->MoveRight();

            return nullptr;
        }

        if (Action->GetName() == "MOVE_LEFT")
        {
            Player->MoveLeft();

            return nullptr;
        }

        if (Action->GetName() == "MOVE_JUMP" && Player->Jump())
        {
            return New<PlayerJumpState>();
        }
    }

    if (ButtonEvent == INPUT_TYPE::UP && (Action->GetName() == "MOVE_RIGHT" || Action->GetName() == "MOVE_LEFT"))
    {
        Player->MoveStop();

        return New<PlayerIdleState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerWalkState::GetAnimationType() const
{
    return ePlayerAnimationType::Walk;
}
