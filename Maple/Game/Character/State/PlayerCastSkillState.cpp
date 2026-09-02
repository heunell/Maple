#include "pch.h"
#include "PlayerCastSkillState.h"
#include "PlayerNoneActionState.h"
#include "Component/PlayerComponent.h"
#include "Component/SkillComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

Ptr<PlayerState> PlayerCastSkillState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    return nullptr;
}

void PlayerCastSkillState::Enter(Ptr<class PlayerComponent> PlayerComponent)
{
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

void PlayerCastSkillState::Exit(Ptr<class PlayerComponent> PlayerComponent)
{
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
        Skill->StopSkill(_SkillType);
    }
}

Ptr<PlayerState> PlayerCastSkillState::Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime)
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

    Skill->UseSkill(_SkillType, DeltaTime);

    Ptr<SpriteComponent> SkillSprite = Player->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite || !SkillSprite->IsEnable())
    {
        return New<PlayerNoneActionState>();
    }

    Ptr<Animation2D> Animation = SkillSprite->GetAnimation();

    if (Animation && Animation->IsFinished())
    {
        return New<PlayerNoneActionState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerCastSkillState::GetAnimationType() const
{
    return ePlayerAnimationType::Shoot;
}