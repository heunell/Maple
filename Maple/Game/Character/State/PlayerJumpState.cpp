#include "pch.h"
#include "PlayerJumpState.h"
#include "Component/PlayerComponent.h"
#include "Component/MovementComponent.h"
#include "Game/Character/Player.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "Game/Skills/WindJump.h"
#include "World/World.h"
#include "World/Level.h"

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

    if (Action->GetName() == "MOVE_UP")
    {
        if (ButtonEvent == INPUT_TYPE::HOLD)
        {
            _IsUpPressed = true;
        }
        else if (ButtonEvent == INPUT_TYPE::UP)
        {
            _IsUpPressed = false;
        }

        return nullptr;
    }

    // 첫 점프 키를 놓아야 공중점프를 사용할 수 있다.
    if (Action->GetName() == "MOVE_JUMP" && ButtonEvent == INPUT_TYPE::UP)
    {
        if (!_UsedAirJump)
        {
            _CanAirJump = true;
        }

        return nullptr;
    }

    if (Action->GetName() == "MOVE_JUMP" && ButtonEvent == INPUT_TYPE::HOLD && _CanAirJump && !_UsedAirJump)
    {
        if (_IsUpPressed)
        {
            if (Player->UpJump())
            {
                DrawWindJumpEffect(Player, eWindJumpType::UpBack);

                DrawWindJumpEffect(Player, eWindJumpType::UpSpecial);

                DrawWindJumpEffect(Player, eWindJumpType::UpFront);

                _CanAirJump = false;
                _UsedAirJump = true;
            }
        }
        else if (Player->DoubleJump())
        {
            DrawWindJumpEffect(Player, eWindJumpType::DoubleBack);

            DrawWindJumpEffect(Player, eWindJumpType::DoubleFront);

            _CanAirJump = false;
            _UsedAirJump = true;
        }

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

void PlayerJumpState::DrawWindJumpEffect(Ptr<class Player> Player, eWindJumpType Type)
{
    if (!Player)
    {
        return;
    }

    Ptr<Level> CurrentLevel = Player->GetLevel();

    if (!CurrentLevel)
    {
        return;
    }

    Ptr<WindJump> Effect = CurrentLevel->SpawnActor<WindJump>("WindJumpEffect", Player->GetWorldPosition(), FVector3D(1.f, 1.f, 1.f), FRotator());

    if (Effect)
    {
        Effect->Start(Type, Player->IsRight());
    }
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
