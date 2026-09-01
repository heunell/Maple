#pragma once

#include "PlayerState.h"

class PlayerProneState : public PlayerState
{
public:
    PlayerProneState() = default;
    virtual ~PlayerProneState() = default;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual void Enter(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};