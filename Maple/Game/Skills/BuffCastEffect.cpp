#include "pch.h"
#include "BuffCastEffect.h"
#include "Component/SpriteComponent.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Game/Character/Player.h"

bool BuffCastEffect::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    _CastSprite = CreateSceneComponent<SpriteComponent>("CastSprite");

    if (!_CastSprite)
    {
        return false;
    }

    _CastSpriteOverlay = CreateSceneComponent<SpriteComponent>("CastSpriteOverlay");

    if (!_CastSpriteOverlay)
    {
        return false;
    }

    _CastSprite->AttachToComponent(GetRoot());

    _CastSpriteOverlay->AttachToComponent(GetRoot());

    _CastSprite->SetEnable(false);

    _CastSpriteOverlay->SetEnable(false);

    return true;
}

void BuffCastEffect::Tick(float DeltaTime)
{
    Ptr<Player> Target = Lock(_Target);

    if (!Target)
    {
        Remove();

        return;
    }

    SetWorldPosition(Target->GetWorldPosition());

    Actor::Tick(DeltaTime);

    Ptr<Animation2D> CastAnimation = _CastSprite->GetAnimation();

    if (!CastAnimation || !CastAnimation->IsFinished())
    {
        return;
    }

    if (_CastSpriteOverlay->IsEnable())
    {
        Ptr<Animation2D> OverlayAnimation = _CastSpriteOverlay->GetAnimation();

        if (!OverlayAnimation || !OverlayAnimation->IsFinished())
        {
            return;
        }
    }

    Remove();
}

void BuffCastEffect::Destroy()
{
    _Target.reset();

    _CastSprite = nullptr;

    _CastSpriteOverlay = nullptr;

    Actor::Destroy();
}

bool BuffCastEffect::Start(Ptr<Player> Target, const FBuffCastEffectData& Data)
{
    if (!Target || Data.CastAnimation.empty())
    {
        Remove();

        return false;
    }

    Ptr<Animation2DData> CastData = ANIMATION_MANAGER->FindAnimation(Data.CastAnimation);

    if (!CastData)
    {
        Remove();

        return false;
    }

    Ptr<Animation2DData> OverlayData = nullptr;

    if (!Data.OverlayAnimation.empty())
    {
        OverlayData = ANIMATION_MANAGER->FindAnimation(Data.OverlayAnimation);

        if (!OverlayData)
        {
            Remove();

            return false;
        }
    }

    _Target = Target;

    SetWorldPosition(Target->GetWorldPosition());

    _CastSprite->SetRenderLayerName(Data.CastRenderLayer);

    //_CastSprite->SetRelativePosition(Data.CastOffset);
    
    _CastSprite->AddAnimationSequence(CastData, false);
    
    _CastSprite->SetEnable(true);
    
    _CastSprite->ChangeAnimation(Data.CastAnimation);
    
    _CastSprite->SetAnimationFrame(0);
    
    _CastSprite->SetPlay(Data.CastAnimation, true);

    if (!OverlayData)
    {
        _CastSpriteOverlay->SetEnable(false);

        return true;
    }

    _CastSpriteOverlay->SetRenderLayerName(Data.OverlayRenderLayer);
    
    //_CastSpriteOverlay->SetRelativePosition(Data.OverlayOffset);
    
    _CastSpriteOverlay->AddAnimationSequence(OverlayData, false);
    
    _CastSpriteOverlay->SetEnable(true);
    
    _CastSpriteOverlay->ChangeAnimation(Data.OverlayAnimation);
    
    _CastSpriteOverlay->SetAnimationFrame(0);
    
    _CastSpriteOverlay->SetPlay(Data.OverlayAnimation, true);

    return true;
}