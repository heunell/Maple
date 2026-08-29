#pragma once
#include "PlayerState.h"

class PlayerNoneActionState : public PlayerState
{
public:
    PlayerNoneActionState() = default;
    virtual ~PlayerNoneActionState() = default;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};