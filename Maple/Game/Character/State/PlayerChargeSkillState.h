#pragma once

#include "PlayerState.h"
#include "Game/Skills/Skill.h"

class PlayerChargeSkillState : public PlayerState
{
public:
    PlayerChargeSkillState(eSkillType SkillType, const std::string& ActionName) : _SkillType(SkillType), _ActionName(ActionName) {}
    virtual ~PlayerChargeSkillState() = default;

private:
    eSkillType _SkillType;

    std::string _ActionName;

    bool _IsReleased = false;

public:
    virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

    virtual void Enter(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual void Exit(Ptr<class PlayerComponent> PlayerComponent) override;

    virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;

    virtual ePlayerAnimationType GetAnimationType() const override;
};