#include "pch.h"
#include "PlayerNoneActionState.h"
#include "PlayerShootState.h"
#include "PlayerCastSkillState.h"
#include "PlayerChargeSkillState.h"
#include "Game/Skills/Skill.h"

Ptr<PlayerState> PlayerNoneActionState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action)
    {
        return nullptr;
    }

    if (Action->GetName() == "SongOfHeaven" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerShootState>();
    }

    if (Action->GetName() == "Anemoi" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::Anemoi);
    }

    if (Action->GetName() == "Vortex" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::VortexSphere);
    }

    if (Action->GetName() == "HowlingGale" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerChargeSkillState>(eSkillType::HowlingGale, "HowlingGale");
    }

    if (Action->GetName() == "SharpEyes" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::SharpEyes);
    }

    if (Action->GetName() == "CygnusKnights" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::CKnight);
    }

    if (Action->GetName() == "StormBringer" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::StormBringer);
    }

    if (Action->GetName() == "GloryOfGuardians" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::GloryOfGuardians);
    }

    if (Action->GetName() == "DeSpell" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerCastSkillState>(eSkillType::DeSpell);
    }

    return nullptr;
}


ePlayerAnimationType PlayerNoneActionState::GetAnimationType() const
{
    return ePlayerAnimationType::None;
}