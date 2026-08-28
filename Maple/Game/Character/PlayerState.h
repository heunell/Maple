#pragma once

#include "Core/Object.h"
#include "Component/PlayerComponent.h"
#include "Input/InputAction.h"


class PlayerState : public Object
{
public:
    PlayerState() = default;
    ~PlayerState() = default;

protected:
    uint8 _StateType = -1;

public:
    void Destroy() override;

    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> _Player, Ptr<InputAction> action, INPUT_TYPE::eType ButtonEvent) = 0;

    virtual void Enter(Ptr<class PlayerComponent> _PlayerComponent) {}

    virtual void Exit(Ptr<class PlayerComponent> _PlayerComponent) {}

    virtual void Tick(Ptr<class PlayerComponent> _PlayerComponent, float DeltaTime) {}

    uint8 GetType() const;

};