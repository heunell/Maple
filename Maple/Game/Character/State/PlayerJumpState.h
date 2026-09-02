#pragma once

#include "PlayerState.h"

enum class eWindJumpType;

class PlayerJumpState : public PlayerState
{
public:
    PlayerJumpState() = default;
    virtual ~PlayerJumpState() = default;
    
private:
    bool _CanAirJump = false;

    bool _UsedAirJump = false;

    bool _IsUpPressed = false;

    void DrawWindJumpEffect(Ptr<class Player> Player, eWindJumpType Type);
    
public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;
    
    virtual ePlayerAnimationType GetAnimationType() const override;
};

