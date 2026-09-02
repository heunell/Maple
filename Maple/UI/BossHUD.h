#pragma once
#include "UI.h"

class BossHUD : public UI
{
public:
    BossHUD() = default;
    virtual ~BossHUD() = default;
    BossHUD(const BossHUD&) = delete;
    BossHUD(BossHUD&&) = delete;
    BossHUD& operator=(const BossHUD&) = delete;
    BossHUD& operator=(BossHUD&&) = delete;

private:
    Ptr<class ProgressBar>     _HPBar;

    Ptr<class SpriteComponent> _LineCountDigit;

    Ptr<class SpriteComponent> _RatioHundreds;

    Ptr<class SpriteComponent> _RatioTens;

    Ptr<class SpriteComponent> _RatioOnes;

    Ptr<class SpriteComponent> _DeathCountTens;

    Ptr<class SpriteComponent> _DeathCountOnes;

    Ptr<class SpriteComponent> _TimerMinuteTens;

    Ptr<class SpriteComponent> _TimerMinuteOnes;

    Ptr<class SpriteComponent> _TimerSecondTens;

    Ptr<class SpriteComponent> _TimerSecondOnes;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    virtual void Destroy() override;

    void SetHP(float CurrentHP, float MaxHP);

    void SetRatio(int32 Ratio);

    void SetLineCount(int32 LineCount);

    void SetDeathCount(int32 DeathCount);

    void SetTimer(int32 Minute, int32 Second);
};

