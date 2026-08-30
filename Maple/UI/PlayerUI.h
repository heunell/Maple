#pragma once

#include "UI.h"

class PlayerUI : public UI
{
public:
    PlayerUI();
    virtual ~PlayerUI();
    PlayerUI(const PlayerUI&) = delete;
    PlayerUI(PlayerUI&&) = delete;
    PlayerUI& operator=(const PlayerUI&) = delete;
    PlayerUI& operator=(PlayerUI&&) = delete;

private:
    Ptr<class ProgressBar> _HPBar;

    Ptr<class ProgressBar> _MPBar;

    Ptr<class ProgressBar> _EXPBar;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    virtual void Destroy() override;

    void SetHP(float CurrentHP, float MaxHP);

    void SetMP(float CurrentMP, float MaxMP);

    void SetEXP(float CurrentEXP, float MaxEXP);

private:
    Ptr<class SpriteComponent> CreateSprite(const std::string& ComponentName, const std::string& AnimationName);

    Ptr<class ProgressBar> CreateProgressBar(const std::string& ComponentName, const std::string& AnimationName);
};