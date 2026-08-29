#include "pch.h"
#include "PlayerShootState.h"
#include "Component/PlayerComponent.h"
#include "Component/SkillComponent.h"
#include "Game/Character/Player.h"
#include "PlayerNoneActionState.h"

Ptr<PlayerState> PlayerShootState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action)
    {
        return nullptr;
    }

    if (Action->GetName() == "SongOfHeaven" && ButtonEvent == INPUT_TYPE::UP)
    {
        return New<PlayerNoneActionState>();
    }

    return nullptr;
}

ePlayerAnimationType PlayerShootState::GetAnimationType() const
{
    return ePlayerAnimationType::Shoot;
}

void PlayerShootState::Enter(Ptr<class PlayerComponent> PlayerComponent)
{
    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (Skill)
    {
        Skill->StartSkill();
    }
}

void PlayerShootState::Exit(Ptr<class PlayerComponent> PlayerComponent)
{
    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (Skill)
    {
        Skill->StopSkill();
    }
}

Ptr<PlayerState> PlayerShootState::Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime)
{
    Ptr<Player> Player = PlayerComponent->GetPlayer();

    if (!Player)
    {
        return nullptr;
    }

    Ptr<SkillComponent> Skill = Player->FindActorComponent<SkillComponent>("Skill");

    if (Skill)
    {
        Skill->UseSkill(DeltaTime);
    }

    return nullptr;
}
