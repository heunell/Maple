#include "pch.h"
#include "PlayerProneState.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Game/Character/Player.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

Ptr<PlayerState> PlayerProneState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
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

    if (Action->GetName() == "MOVE_JUMP" && ButtonEvent == INPUT_TYPE::HOLD && Player->DownJump())
    {
        return New<PlayerJumpState>();
    }

    if (Action->GetName() == "MOVE_DOWN" && ButtonEvent == INPUT_TYPE::UP)
    {
        return New<PlayerIdleState>();
    }

    return nullptr;
}

void PlayerProneState::Enter(Ptr<class PlayerComponent> PlayerComponent)
{
    if (!PlayerComponent)
    {
        return;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (Player)
    {
        Player->MoveStop();
    }
}

Ptr<PlayerState> PlayerProneState::Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime)
{
    if (!PlayerComponent)
    {
        return nullptr;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");

    if (Movement && !Movement->IsLaning())
    {
        return New<PlayerJumpState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerProneState::GetAnimationType() const
{
    return ePlayerAnimationType::Prone;
}
