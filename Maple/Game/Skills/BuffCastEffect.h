#pragma once

#include "Object/Actor.h"

struct FBuffCastEffectData
{
    std::string CastAnimation;

    std::string CastRenderLayer = "SkillFront";
    
    std::string OverlayAnimation;
    
    std::string OverlayRenderLayer = "SkillFront";
};

class BuffCastEffect : public Actor
{
public:
    BuffCastEffect() = default;
    virtual ~BuffCastEffect() = default;

private:
    Ptr<class SpriteComponent> _CastSprite;
    
    Ptr<class SpriteComponent> _CastSpriteOverlay;

    Weak<class Player> _Target;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
   
    virtual void Tick(float DeltaTime) override;
   
    virtual void Destroy() override;

    bool Start(Ptr<class Player> Target, const FBuffCastEffectData& Data);
};