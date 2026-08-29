#pragma once

#include "PlayerState.h"

class PlayerJumpState : public PlayerState
{
public:
    PlayerJumpState() = default;
    virtual ~PlayerJumpState() = default;
    
public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;
    
    virtual ePlayerAnimationType GetAnimationType() const override;
};

