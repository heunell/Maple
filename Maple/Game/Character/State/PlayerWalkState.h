#pragma once

#include "PlayerState.h"

class PlayerWalkState : public PlayerState
{
public:
    PlayerWalkState() = default;
    virtual ~PlayerWalkState() = default;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};

