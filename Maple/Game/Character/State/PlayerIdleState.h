#pragma once

#include "PlayerState.h"

class PlayerIdleState : public PlayerState
{
public:
    PlayerIdleState() = default;
    virtual ~PlayerIdleState() = default;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};