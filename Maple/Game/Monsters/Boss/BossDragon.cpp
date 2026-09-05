#include "pch.h"
#include "BossDragon.h"
#include "Component/SpriteComponent.h"

void BossDragon::StartBreathPattern()
{
	if (!_Preparing)
	{
		return;
	}

	_Preparing = false;

	_Breathing = true;

	_ElapsedTime = 0.f;

	for (int32 Index = 0; Index < static_cast<int32>(_BreathSprites.size()); ++Index)
	{
		Ptr<SpriteComponent> BreathSprite = _BreathSprites[Index];

		if (!BreathSprite)
		{
			continue;
		}

		std::string AnimationName = "Dragon.phase1.breath.tile." + std::to_string(Index);

		BreathSprite->SetEnable(true);

		BreathSprite->SetAnimationFrame(0);

		BreathSprite->SetPlay(AnimationName, true);
	}
}

bool BossDragon::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("BossAttack");

	_BodySprite = CreateSceneComponent<SpriteComponent>("BossDragonBody");

	if (!_BodySprite)
	{
		return false;
	}

	_BodySprite->SetRenderLayerName("Default");

	_BodySprite->AddAnimationSequence("Dragon.phase1.action.0", true);

	_BodySprite->AddAnimationSequence("Dragon.phase1.action.1.intro", false);

	_BodySprite->AddAnimationSequence("Dragon.phase1.action.1.breathLoop", true);

	_BodySprite->AddAnimationSequence("Dragon.phase1.action.1.tail", false);

	_BodySprite->AttachToComponent(GetRoot());

	_BodySprite->AddNotify("Dragon.phase1.action.1.intro", _PatternData.BreathStartFrame, this, &BossDragon::StartBreathPattern);

	for (int32 Index = 0; Index < _PatternData.BreathCount; ++Index)
	{
		Ptr<SpriteComponent> BreathSprite = CreateSceneComponent<SpriteComponent>("BossDragonBreath" + std::to_string(Index));

		if (!BreathSprite)
		{
			return false;
		}

		std::string AnimationName = "Dragon.phase1.breath.tile." + std::to_string(Index);

		BreathSprite->SetRenderLayerName("Default");

		BreathSprite->AddAnimationSequence(AnimationName, true);

		BreathSprite->AttachToComponent(GetRoot());

		BreathSprite->SetRelativePosition(0.f, 0.f, 0.1f);

		_BreathSprites.push_back(BreathSprite);
	}

	SetPatternEnable(false);

	return true;
}

void BossDragon::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	if (_Appearing)
	{
		_ElapsedTime += DeltaTime;

		float AppearanceRatio = _ElapsedTime / _PatternData.AppearanceTime;

		if (AppearanceRatio > 1.f)
		{
			AppearanceRatio = 1.f;
		}

		FVector3D Position = _StartPosition;

		Position._x += (_DragonPosition._x - _StartPosition._x) * AppearanceRatio;

		Position._y += (_DragonPosition._y - _StartPosition._y) * AppearanceRatio;

		SetWorldPosition(Position);

		if (AppearanceRatio < 1.f)
		{
			return;
		}

		_Appearing = false;

		_Preparing = true;

		_BodySprite->ChangeAnimation("Dragon.phase1.action.1.intro");

		_BodySprite->SetAnimationFrame(0);

		_BodySprite->SetPlay("Dragon.phase1.action.1.intro", true);

		return;
	}

	Ptr<Animation2D> Animation = _BodySprite->GetAnimation();

	if (_Preparing)
	{
		return;
	}

	if (_Breathing)
	{
		if (Animation && Animation->IsFinished())
		{
			_BodySprite->ChangeAnimation("Dragon.phase1.action.1.breathLoop");

			_BodySprite->SetAnimationFrame(0);

			_BodySprite->SetPlay("Dragon.phase1.action.1.breathLoop", true);
		}

		_ElapsedTime += DeltaTime;

		if (_ElapsedTime < _PatternData.BreathTime)
		{
			return;
		}

		_Breathing = false;

		_Ending = true;

		for (Ptr<SpriteComponent>& BreathSprite : _BreathSprites)
		{
			BreathSprite->SetEnable(false);
		}

		_BodySprite->ChangeAnimation("Dragon.phase1.action.1.tail");

		_BodySprite->SetAnimationFrame(0);

		_BodySprite->SetPlay("Dragon.phase1.action.1.tail", true);

		return;
	}

	if (!_Ending)
	{
		return;
	}

	if (!Animation || !Animation->IsFinished())
	{
		return;
	}

	SetPatternEnable(false);
}

void BossDragon::Start(const FVector3D& StartPosition, const FVector3D& DragonPosition, bool LeftSide)
{
	_StartPosition = StartPosition;

	_DragonPosition = DragonPosition;

	_ElapsedTime = 0.f;

	_Appearing = true;

	_Preparing = false;

	_Breathing = false;

	_Ending = false;

	SetWorldPosition(_StartPosition);

	SetPatternEnable(true);

	// 원본 Dragon Body는 왼쪽을 바라보므로 왼쪽에서 등장할 때 뒤집는다.
	_BodySprite->SetAnimationFlip(LeftSide);

	_BodySprite->ChangeAnimation("Dragon.phase1.action.0");

	_BodySprite->SetAnimationFrame(0);

	_BodySprite->SetPlay("Dragon.phase1.action.0", true);

	for (Ptr<SpriteComponent>& BreathSprite : _BreathSprites)
	{
		BreathSprite->SetAnimationFlip(LeftSide);

		BreathSprite->SetEnable(false);
	}
}

void BossDragon::SetPatternEnable(bool Enable)
{
	SetEnable(Enable);

	if (_BodySprite)
	{
		_BodySprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		for (Ptr<SpriteComponent>& BreathSprite : _BreathSprites)
		{
			if (BreathSprite)
			{
				BreathSprite->SetEnable(false);
			}
		}

		_ElapsedTime = 0.f;

		_Appearing = false;

		_Preparing = false;

		_Breathing = false;

		_Ending = false;
	}
}

void BossDragon::Destroy()
{
	_BreathSprites.clear();

	_BodySprite.reset();

	Actor::Destroy();
}