#pragma once

#include "PlayerState.h"
#include "Game/Skills/Skill.h"

class PlayerCastSkillState : public PlayerState
{
public:
    PlayerCastSkillState(eSkillType SkillType) : _SkillType(SkillType) {}
    virtual ~PlayerCastSkillState() = default;

private:
    eSkillType _SkillType;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual void Enter(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual void Exit(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};