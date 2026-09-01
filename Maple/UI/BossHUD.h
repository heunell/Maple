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
    Ptr<class ProgressBar> _HPBar;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    virtual void Destroy() override;

    void SetHP(float CurrentHP, float MaxHP);
};

