#include "pch.h"
#include "BossPatternNotice.h"
#include "TextBlock.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"

bool BossPatternNotice::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!UI::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("UI\\PatternNotice.json"), "Animations"))
	{
		return false;
	}

	Ptr<SceneComponent> LeftSlot = CreateSceneComponent<SceneComponent>("BossPatternNoticeLeftSlot");

	Ptr<SceneComponent> CenterSlot = CreateSceneComponent<SceneComponent>("BossPatternNoticeCenterSlot");

	Ptr<SceneComponent> RightSlot = CreateSceneComponent<SceneComponent>("BossPatternNoticeRightSlot");

	if (!LeftSlot || !CenterSlot || !RightSlot)
	{
		return false;
	}

	LeftSlot->AttachToComponent(GetRoot());

	CenterSlot->AttachToComponent(GetRoot());

	RightSlot->AttachToComponent(GetRoot());

	_LeftSprite = CreateSprite("BossPatternNoticeLeft", "PATTERN_NOTICE.LEFT", LeftSlot);

	if (!_LeftSprite)
	{
		return false;
	}

	_CenterSprite = CreateSprite("BossPatternNoticeCenter", "PATTERN_NOTICE.CENTER", CenterSlot);

	if (!_CenterSprite)
	{
		return false;
	}

	_RightSprite = CreateSprite("BossPatternNoticeRight", "PATTERN_NOTICE.RIGHT", RightSlot);

	if (!_RightSprite)
	{
		return false;
	}

	float CenterWidth = _NoticeData.TotalWidth - _NoticeData.LeftWidth - _NoticeData.RightWidth;

	float LeftPositionX = -_NoticeData.TotalWidth * 0.5f + _NoticeData.LeftWidth * 0.5f;

	float CenterPositionX = -_NoticeData.TotalWidth * 0.5f + _NoticeData.LeftWidth + CenterWidth * 0.5f;

	float RightPositionX = _NoticeData.TotalWidth * 0.5f - _NoticeData.RightWidth * 0.5f;

	LeftSlot->SetRelativePosition(LeftPositionX, 0.f, 0.f);

	CenterSlot->SetRelativePosition(CenterPositionX, 0.f, 0.f);

	RightSlot->SetRelativePosition(RightPositionX - 1.f, 0.f, 0.f);

	_TextBlock = CreateTextBlock("BossPatternNoticeText");

	if (!_TextBlock)
	{
		return false;
	}

	_TextBlock->SetRelativePosition(35.f, -15.f, 0.f);

	_TextBlock->SetSize(FVector2D(420.f, 40.f));

	_TextBlock->SetFontSize(18.f);

	_TextBlock->SetAlignH(eTextAlignH::CENTER);

	_TextBlock->SetAlignV(eTextAlignV::MIDDLE);

	_TextBlock->SetTextColor(255, 255, 255, 255);

	_TextBlock->SetText(L"");

	SetNoticeEnable(false);

	return true;
}

void BossPatternNotice::Tick(float DeltaTime)
{
	UI::Tick(DeltaTime);

	if (_Finished)
	{
		return;
	}

	_ElapsedTime += DeltaTime;

	float Opacity = 1.f;

	if (_ElapsedTime < _NoticeData.FadeInTime)
	{
		Opacity = _ElapsedTime / _NoticeData.FadeInTime;
	}
	else if (_ElapsedTime >= _NoticeData.FadeInTime + _NoticeData.HoldTime)
	{
		float FadeOutElapsedTime = _ElapsedTime - _NoticeData.FadeInTime - _NoticeData.HoldTime;

		Opacity = 1.f - FadeOutElapsedTime / _NoticeData.FadeOutTime;
	}

	float TotalTime = _NoticeData.FadeInTime + _NoticeData.HoldTime + _NoticeData.FadeOutTime;

	if (_ElapsedTime >= TotalTime)
	{
		_Finished = true;

		SetNoticeEnable(false);

		return;
	}

	if (Opacity < 0.f)
	{
		Opacity = 0.f;
	}

	if (Opacity > 1.f)
	{
		Opacity = 1.f;
	}

	SetNoticeOpacity(Opacity);
}

void BossPatternNotice::Show(const std::wstring& Text)
{
	if (_TextBlock)
	{
		_TextBlock->SetText(Text);
	}

	_ElapsedTime = 0.f;

	_Finished = false;

	SetNoticeOpacity(0.f);

	SetNoticeEnable(true);
}

bool BossPatternNotice::IsFinished() const
{
	return _Finished;
}

void BossPatternNotice::SetNoticeEnable(bool Enable)
{
	SetEnable(Enable);

	if (_LeftSprite)
	{
		_LeftSprite->SetEnable(Enable);
	}

	if (_CenterSprite)
	{
		_CenterSprite->SetEnable(Enable);
	}

	if (_RightSprite)
	{
		_RightSprite->SetEnable(Enable);
	}

	if (_TextBlock)
	{
		_TextBlock->SetEnable(Enable);
	}
}

void BossPatternNotice::SetNoticeOpacity(float Opacity)
{
	_LeftSprite->SetOpacity(Opacity);

	_CenterSprite->SetOpacity(Opacity);

	_RightSprite->SetOpacity(Opacity);

	_TextBlock->SetOpacity(Opacity);
}

void BossPatternNotice::Destroy()
{
	_LeftSprite.reset();

	_CenterSprite.reset();

	_RightSprite.reset();

	_TextBlock.reset();

	UI::Destroy();
}