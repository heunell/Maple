#pragma once

#include "Core/Object.h"
#include "Input/InputAction.h"

enum class ePlayerAnimationType
{
    None,
    Idle, // = stand
    Walk,
    Jump,
    Shoot,
    Prone,
    Dead,
    Alter, // 공격이나 피격 이후에 이뤄지는 3초짜리 모션

    End
};
class PlayerState : public Object
{
public:
    PlayerState() = default;
    ~PlayerState() = default;

protected:
    uint8 _StateType = static_cast<uint8>(-1);

public:
    virtual void Destroy() override;

    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> _Player, Ptr<InputAction> action, INPUT_TYPE::eType ButtonEvent) = 0;

    virtual void Enter(Ptr<class PlayerComponent> _PlayerComponent) {}

    virtual void Exit(Ptr<class PlayerComponent> _PlayerComponent) {}

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> _PlayerComponent, float DeltaTime);

    virtual ePlayerAnimationType GetAnimationType() const = 0;

    uint8 GetType() const;
};