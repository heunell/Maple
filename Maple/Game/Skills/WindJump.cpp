#include "pch.h"
#include "WindJump.h"
#include "Component/SpriteComponent.h"

bool WindJump::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    _Sprite = CreateSceneComponent<SpriteComponent>("WindJumpEffectSprite");

    if (!_Sprite)
    {
        return false;
    }

    _Sprite->AddAnimationSequence("WindJump.DoubleFront", false);

    _Sprite->AddAnimationSequence("WindJump.DoubleBack", false);

    _Sprite->AddAnimationSequence("WindJump.UpFront", false);

    _Sprite->AddAnimationSequence("WindJump.UpBack", false);

    _Sprite->AddAnimationSequence("WindJump.UpSpecial", false);

    _Sprite->AttachToComponent(GetRoot());

    return true;
}

void WindJump::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);

    if (!_Sprite)
    {
        return;
    }

    Ptr<Animation2D> Animation = _Sprite->GetAnimation();

    if (Animation && Animation->IsFinished())
    {
        Remove();
    }
}

void WindJump::Start(eWindJumpType Type, bool IsRight)
{
    if (!_Sprite)
    {
        return;
    }

    std::string AnimationName;

    switch (Type)
    {
    case eWindJumpType::DoubleFront:
        AnimationName = "WindJump.DoubleFront";
        _Sprite->SetRenderLayerName("SkillFront");
        _Sprite->SetAnimationFlip(IsRight);
        break;

    case eWindJumpType::DoubleBack:
        AnimationName = "WindJump.DoubleBack";
        _Sprite->SetRenderLayerName("SkillBack");
        _Sprite->SetAnimationFlip(IsRight);
        break;

    case eWindJumpType::UpFront:
        AnimationName = "WindJump.UpFront";
        _Sprite->SetRenderLayerName("SkillFront");
        _Sprite->SetAnimationFlip(!IsRight);
        break;

    case eWindJumpType::UpBack:
        AnimationName = "WindJump.UpBack";
        _Sprite->SetRenderLayerName("SkillBack");
        _Sprite->SetAnimationFlip(!IsRight);
        break;

    case eWindJumpType::UpSpecial:
        AnimationName = "WindJump.UpSpecial";
        _Sprite->SetRenderLayerName("SkillBack");
        _Sprite->SetAnimationFlip(!IsRight);
        break;

    default:
        return;
    }

    _Sprite->ChangeAnimation(AnimationName);

    _Sprite->SetAnimationFrame(0);

    _Sprite->SetPlay(AnimationName, true);
}