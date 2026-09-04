#include "pch.h"
#include "PlayerChargeSkillState.h"
#include "PlayerNoneActionState.h"
#include "Component/PlayerComponent.h"
#include "Component/SkillComponent.h"
#include "Game/Character/Player.h"

Ptr<PlayerState> PlayerChargeSkillState::HandleInput(Ptr<PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action)
    {
        return nullptr;
    }

    if (Action->GetName() != _ActionName || ButtonEvent != INPUT_TYPE::UP)
    {
        return nullptr;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (!Skill)
    {
        return New<PlayerNoneActionState>();
    }

    Skill->StopSkill(_SkillType);

    _IsReleased = true;

    return nullptr;
}

void PlayerChargeSkillState::Enter(Ptr<PlayerComponent> PlayerComponent)
{
    _IsReleased = false;

    if (!PlayerComponent)
    {
        return;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (Skill)
    {
        Skill->StartSkill(_SkillType);
    }
}

void PlayerChargeSkillState::Exit(Ptr<PlayerComponent> PlayerComponent)
{
    if (_IsReleased || !PlayerComponent)
    {
        return;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (Skill)
    {
        Skill->StopSkill(_SkillType);
    }
}

Ptr<PlayerState> PlayerChargeSkillState::Tick(Ptr<PlayerComponent> PlayerComponent, float DeltaTime)
{
    if (!PlayerComponent)
    {
        return nullptr;
    }

    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (!Skill)
    {
        return New<PlayerNoneActionState>();
    }

    if (!_IsReleased)
    {
        Skill->UseSkill(_SkillType, DeltaTime);
    }

    if (!Skill->IsSkillActive(_SkillType))
    {
        return New<PlayerNoneActionState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerChargeSkillState::GetAnimationType() const
{
    return ePlayerAnimationType::Shoot;
}