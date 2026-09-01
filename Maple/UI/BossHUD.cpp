#include "pch.h"
#include "BossHUD.h"
#include "ProgressBar.h"

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

    return true;
}

void BossHUD::SetHP(float CurrentHP, float MaxHP)
{
	if(_HPBar)
	{
		_HPBar->SetValue(CurrentHP, MaxHP);
	}
}

void BossHUD::Destroy()
{
	_HPBar = nullptr;

	UI::Destroy();
}