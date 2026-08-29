#pragma once

#include "PlayerState.h"

class PlayerShootState : public PlayerState
{
public:
    PlayerShootState() = default;
    virtual ~PlayerShootState() = default;
    
public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual ePlayerAnimationType GetAnimationType() const override;

    virtual void Enter(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual void Exit(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;
};

