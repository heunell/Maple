#include "pch.h"
#include "PlayerJumpState.h"
#include "Component/PlayerComponent.h"
#include "Component/MovementComponent.h"
#include "Game/Character/Player.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"

Ptr<PlayerState> PlayerJumpState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
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
        if (Action->GetName() == "MOVE_RIGHT")
        {
            Player->MoveRight();
        }
        else if (Action->GetName() == "MOVE_LEFT")
        {
            Player->MoveLeft();
        }
    }

    if (ButtonEvent == INPUT_TYPE::UP && (Action->GetName() == "MOVE_RIGHT" || Action->GetName() == "MOVE_LEFT"))
    {
        Player->MoveStop();
    }

    return nullptr;
}

Ptr<PlayerState> PlayerJumpState::Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime)
{
    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");

    if (!Movement || !Movement->IsLaning())
    {
        return nullptr;
    }

    if (Movement->GetMoveAxis()._x != 0.f)
    {
        return New<PlayerWalkState>();
    }

    return New<PlayerIdleState>();
}

ePlayerAnimationType PlayerJumpState::GetAnimationType() const
{
    return ePlayerAnimationType::Jump;
}
