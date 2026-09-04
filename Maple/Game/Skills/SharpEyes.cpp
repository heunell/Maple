#include "pch.h"
#include "SharpEyes.h"
#include "BuffCastEffect.h"
#include "Component/BuffComponent.h"
#include "Game/Character/Player.h"
#include "World/Level.h"

bool SharpEyes::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    _CoolDown = 30.f;

    return true;
}

void SharpEyes::Collision(float DeltaTime)
{}

void SharpEyes::Start()
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

    Ptr<BuffCastEffect> Effect = Level->SpawnActor<BuffCastEffect>("SharpEyesCastEffect", PlayerOwner->GetWorldPosition(), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

    if (!Effect)
    {
        return;
    }

    FBuffCastEffectData EffectData;

    EffectData.CastAnimation = "SharpEyes.Cast";
    
    EffectData.CastRenderLayer = "SkillFront";

    if (!Effect->Start(PlayerOwner, EffectData))
    {
        return;
    }

    FBuffData BuffData;
   
    BuffData.Type = eBuffType::SharpEyes;
    
    BuffData.Category = eBuffCategory::Buff;
    
    BuffData.Duration = 180.f;

    if (!Buff->ApplyBuff(BuffData))
    {
        Effect->Remove();
       
        return;
    }

    StartCoolDown();
}

void SharpEyes::Update(float DeltaTime)
{}

void SharpEyes::End()
{}

void SharpEyes::Destroy()
{
    Skill::Destroy();
}
