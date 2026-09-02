#include "pch.h"
#include "BossHUD.h"
#include "ProgressBar.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"

bool BossHUD::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!UI::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

	SetWorldPosition(Position);
	
    if (!CreateSprite("BossHPFrame", "LUCID_BOSS_UI.HP.FRAME"))
    {
        return false;
    }

    if (!CreateSprite("BossHPNull", "LUCID_BOSS_UI.HP.NULL"))
    {
        return false;
    }

    _HPBar = CreateProgressBar("BossHPBar", "LUCID_BOSS_UI.HP.FILL");

    if (!_HPBar)
    {
        return false;
    }

    if (!CreateSprite("BossHPPortrait", "LUCID_BOSS_UI.HP.PORTRAIT"))
    {
        return false;
    }

    if (!CreateSprite("BossHPRatioBackground", "LUCID_BOSS_UI.HP.RATIO_BG"))
    {
        return false;
    }

    if (!CreateSprite("BossHPLineCountBackground", "LUCID_BOSS_UI.LINE_COUNT.BG"))
    {
        return false;
    }

    _LineCountDigit = CreateSprite("BossHPLineCountDigit", "LUCID_BOSS_UI.LINE_COUNT.0");

    if (!_LineCountDigit)
    {
        return false;
    }

    Ptr<SceneComponent> RatioSlot = CreateSceneComponent<SceneComponent>("BossHPRatioHundredsSlot");

    if (!RatioSlot)
    {
        return false;
    }

    RatioSlot->AttachToComponent(GetRoot());

    RatioSlot->SetRelativePosition(288.5f, 721.5f, 0.f);

    _RatioHundreds = CreateSprite("BossHPRatioHundreds", "LUCID_BOSS_UI.HP.PERCENT_0", RatioSlot);

    if (!_RatioHundreds)
    {
        return false;
    }

    RatioSlot = CreateSceneComponent<SceneComponent>("BossHPRatioTensSlot");

    if (!RatioSlot)
    {
        return false;
    }

    RatioSlot->AttachToComponent(GetRoot());

    RatioSlot->SetRelativePosition(294.5f, 721.5f, 0.f);

    _RatioTens = CreateSprite("BossHPRatioTens", "LUCID_BOSS_UI.HP.PERCENT_0", RatioSlot);

    if (!_RatioTens)
    {
        return false;
    }

    RatioSlot = CreateSceneComponent<SceneComponent>("BossHPRatioOnesSlot");

    if (!RatioSlot)
    {
        return false;
    }

    RatioSlot->AttachToComponent(GetRoot());

    RatioSlot->SetRelativePosition(300.5f, 721.5f, 0.f);

    _RatioOnes = CreateSprite("BossHPRatioOnes", "LUCID_BOSS_UI.HP.PERCENT_0", RatioSlot);

    if (!_RatioOnes)
    {
        return false;
    }

    RatioSlot = CreateSceneComponent<SceneComponent>("BossHPRatioPercentSlot");

    if (!RatioSlot)
    {
        return false;
    }

    RatioSlot->AttachToComponent(GetRoot());

    RatioSlot->SetRelativePosition(307.5f, 721.5f, 0.f);

    if (!CreateSprite("BossHPRatioPercent", "LUCID_BOSS_UI.HP.PERCENT", RatioSlot))
    {
        return false;
    }

    Ptr<SceneComponent> DeathCountSlot = CreateSceneComponent<SceneComponent>("BossDeathCountFrameSlot");

    if (!DeathCountSlot)
    {
        return false;
    }

    DeathCountSlot->AttachToComponent(GetRoot());

    DeathCountSlot->SetRelativePosition(524.5f, 698.f, 0.f);

    if (!CreateSprite("BossDeathCountFrame", "LUCID_EXTRA.PLAYER_DEATH_COUNT.FRAME", DeathCountSlot))
    {
        return false;
    }

    DeathCountSlot = CreateSceneComponent<SceneComponent>("BossDeathCountTensSlot");

    if (!DeathCountSlot)
    {
        return false;
    }

    DeathCountSlot->AttachToComponent(GetRoot());

    DeathCountSlot->SetRelativePosition(543.f, 686.f, 0.f);

    _DeathCountTens = CreateSprite("BossDeathCountTens", "LUCID_EXTRA.PLAYER_DEATH_COUNT.0", DeathCountSlot);

    if (!_DeathCountTens)
    {
        return false;
    }

    DeathCountSlot = CreateSceneComponent<SceneComponent>("BossDeathCountOnesSlot");

    if (!DeathCountSlot)
    {
        return false;
    }

    DeathCountSlot->AttachToComponent(GetRoot());

    DeathCountSlot->SetRelativePosition(555.f, 686.f, 0.f);

    _DeathCountOnes = CreateSprite("BossDeathCountOnes", "LUCID_EXTRA.PLAYER_DEATH_COUNT.0", DeathCountSlot);

    if (!_DeathCountOnes)
    {
        return false;
    }

    for (int32 Number = 1; Number <= 9; ++Number)
    {
        _LineCountDigit->AddAnimationSequence("LUCID_BOSS_UI.LINE_COUNT." + std::to_string(Number));

        _RatioHundreds->AddAnimationSequence("LUCID_BOSS_UI.HP.PERCENT_" + std::to_string(Number));

        _RatioTens->AddAnimationSequence("LUCID_BOSS_UI.HP.PERCENT_" + std::to_string(Number));

        _RatioOnes->AddAnimationSequence("LUCID_BOSS_UI.HP.PERCENT_" + std::to_string(Number));

        _DeathCountTens->AddAnimationSequence("LUCID_EXTRA.PLAYER_DEATH_COUNT." + std::to_string(Number));

        _DeathCountOnes->AddAnimationSequence("LUCID_EXTRA.PLAYER_DEATH_COUNT." + std::to_string(Number));
    }

    if (!CreateSprite("BossTimerFrame", "BOSS_TIMER.FRAME"))
    {
        return false;
    }

    _TimerMinuteTens = CreateSprite("BossTimerMinuteTens", "BOSS_TIMER.MINUTE_TENS");

    if (!_TimerMinuteTens)
    {
        return false;
    }

    _TimerMinuteOnes = CreateSprite("BossTimerMinuteOnes", "BOSS_TIMER.MINUTE_ONES" );

    if (!_TimerMinuteOnes)
    {
        return false;
    }

    _TimerSecondTens = CreateSprite("BossTimerSecondTens", "BOSS_TIMER.SECOND_TENS");

    if (!_TimerSecondTens)
    {
        return false;
    }

    _TimerSecondOnes = CreateSprite("BossTimerSecondOnes", "BOSS_TIMER.SECOND_ONES" );

    if (!_TimerSecondOnes)
    {
        return false;
    }

    _TimerMinuteTens->SetPlay("BOSS_TIMER.MINUTE_TENS", false);

    _TimerMinuteOnes->SetPlay("BOSS_TIMER.MINUTE_ONES", false);

    _TimerSecondTens->SetPlay("BOSS_TIMER.SECOND_TENS", false);

    _TimerSecondOnes->SetPlay("BOSS_TIMER.SECOND_ONES", false);

    _RatioHundreds->SetEnable(false);

    _RatioTens->SetEnable(false);

    return true;
}

void BossHUD::SetHP(float CurrentHP, float MaxHP)
{
	if(_HPBar)
	{
		_HPBar->SetValue(CurrentHP, MaxHP);
	}
}

void BossHUD::SetRatio(int32 Ratio)
{
    if (!_RatioHundreds || !_RatioTens || !_RatioOnes)
    {
        return;
    }

    if (Ratio < 0 || Ratio > 100)
    {
        return;
    }

    if (Ratio == 100)
    {
        _RatioHundreds->SetEnable(true);

        _RatioTens->SetEnable(true);
        
        _RatioOnes->SetEnable(true);

        _RatioHundreds->ChangeAnimation("LUCID_BOSS_UI.HP.PERCENT_1");

        _RatioTens->ChangeAnimation("LUCID_BOSS_UI.HP.PERCENT_0");

        _RatioOnes->ChangeAnimation("LUCID_BOSS_UI.HP.PERCENT_0");

        return;
    }

    _RatioHundreds->SetEnable(false);

    if (Ratio >= 10)
    {
        _RatioTens->SetEnable(true);

        _RatioTens->ChangeAnimation("LUCID_BOSS_UI.HP.PERCENT_" + std::to_string(Ratio / 10));
    }
    else
    {
        _RatioTens->SetEnable(false);
    }

    _RatioOnes->SetEnable(true);

    _RatioOnes->ChangeAnimation("LUCID_BOSS_UI.HP.PERCENT_" + std::to_string(Ratio % 10));
}

void BossHUD::SetLineCount(int32 LineCount)
{
    if (!_LineCountDigit)
    {
        return;
    }

    if (LineCount < 0 || LineCount > 9)
    {
        return;
    }

    _LineCountDigit->ChangeAnimation("LUCID_BOSS_UI.LINE_COUNT." + std::to_string(LineCount));
}

void BossHUD::SetDeathCount(int32 DeathCount)
{
    if (!_DeathCountTens || !_DeathCountOnes)
    {
        return;
    }

    if (DeathCount < 0 || DeathCount > 99)
    {
        return;
    }

    _DeathCountTens->ChangeAnimation("LUCID_EXTRA.PLAYER_DEATH_COUNT." + std::to_string(DeathCount / 10));

    _DeathCountOnes->ChangeAnimation("LUCID_EXTRA.PLAYER_DEATH_COUNT." + std::to_string(DeathCount % 10));
}

void BossHUD::SetTimer(int32 Minute, int32 Second)
{
    if (!_TimerMinuteTens || !_TimerMinuteOnes || !_TimerSecondTens || !_TimerSecondOnes)
    {
        return;
    }

    if (Minute < 0 || Minute > 99 || Second < 0 || Second > 59)
    {
        return;
    }

    _TimerMinuteTens->SetAnimationFrame(Minute / 10);

    _TimerMinuteOnes->SetAnimationFrame(Minute % 10);

    _TimerSecondTens->SetAnimationFrame(Second / 10);

    _TimerSecondOnes->SetAnimationFrame(Second % 10);
}

void BossHUD::Destroy()
{
    _LineCountDigit = nullptr;

    _RatioHundreds   = nullptr;

    _RatioTens       = nullptr;
    
    _RatioOnes       = nullptr;

    _DeathCountTens  = nullptr;

    _DeathCountOnes  = nullptr;

    _HPBar           = nullptr;

    _TimerMinuteTens = nullptr;

    _TimerMinuteOnes = nullptr;

    _TimerSecondTens = nullptr;

    _TimerSecondOnes = nullptr;

    UI::Destroy();
}