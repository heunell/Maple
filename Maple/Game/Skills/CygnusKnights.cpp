#include "pch.h"
#include "CygnusKnights.h"
#include "BuffCastEffect.h"
#include "Component/BuffComponent.h"
#include "Game/Character/Player.h"
#include "World/Level.h"

bool CygnusKnights::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    _CoolDown = 30.f;

    return true;
}

void CygnusKnights::Collision(float DeltaTime)
{}

void CygnusKnights::Start()
{
    if (!CanUse())
    {
        return;
    }

    Ptr<Actor> Owner = Lock(_Owner);
    
    if (!Owner)
    {
        return;
    }

    Ptr<Player> PlayerOwner = Cast<Actor, Player>(Owner);

    Ptr<BuffComponent> Buff = Owner->FindActorComponent<BuffComponent>("Buff");

    if (!PlayerOwner || !Buff)
    {
        return;
    }

    Ptr<Level> Level = PlayerOwner->GetLevel();

    if (!Level)
    {
        return;
    }

    Ptr<BuffCastEffect> Effect = Level->SpawnActor<BuffCastEffect>("CygnusKnightsCastEffect", PlayerOwner->GetWorldPosition(), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

    if (!Effect)
    {
        return;
    }

    FBuffCastEffectData EffectData;

    EffectData.CastAnimation = "CKnight.Cast";

    EffectData.CastRenderLayer = "SkillFront";

    EffectData.OverlayAnimation = "CKnight.Aura";

    EffectData.OverlayRenderLayer = "SkillFront";

    if (!Effect->Start(PlayerOwner, EffectData))
    {
        return;
    }

    FBuffData BuffData;

    BuffData.Type = eBuffType::CKnight;

    BuffData.Category = eBuffCategory::Buff;

    BuffData.Duration = 180.f;

    if (!Buff->ApplyBuff(BuffData))
    {
        Effect->Remove();

        return;
    }

    StartCoolDown();
}

void CygnusKnights::Update(float DeltaTime)
{}

void CygnusKnights::End()
{}

void CygnusKnights::Destroy()
{
    Skill::Destroy();
}
