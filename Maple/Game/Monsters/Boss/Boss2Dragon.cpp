#include "pch.h"
#include "Boss2Dragon.h"
#include "Component/SpriteComponent.h"

void Boss2Dragon::StartBreathPattern()
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

		std::string AnimationName = "Dragon.phase2.breath.tile." + std::to_string(Index);

		BreathSprite->SetEnable(true);

		BreathSprite->SetAnimationFrame(0);

		BreathSprite->SetPlay(AnimationName, true);
	}
}

bool Boss2Dragon::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("BossAttack");

	_BodySprite = CreateSceneComponent<SpriteComponent>("Boss2DragonBody");

	if (!_BodySprite)
	{
		return false;
	}

	_BodySprite->SetRenderLayerName("Default");

	_BodySprite->AddAnimationSequence("Dragon.phase2.action.0", true);

	_BodySprite->AddAnimationSequence("Dragon.phase2.action.1.intro", false);

	_BodySprite->AddAnimationSequence("Dragon.phase2.action.1.breathLoop", true);

	_BodySprite->AddAnimationSequence("Dragon.phase2.action.1.tail", false);

	_BodySprite->AttachToComponent(GetRoot());

	_BodySprite->SetAnimationFlip(true);

	_BodySprite->AddNotify("Dragon.phase2.action.1.intro", _PatternData.BreathStartFrame, this, &Boss2Dragon::StartBreathPattern);

	for (int32 Index = 0; Index < _PatternData.BreathCount; ++Index)
	{
		Ptr<SpriteComponent> BreathSprite = CreateSceneComponent<SpriteComponent>("Boss2DragonBreath" + std::to_string(Index));

		if (!BreathSprite)
		{
			return false;
		}

		std::string AnimationName = "Dragon.phase2.breath.tile." + std::to_string(Index);

		BreathSprite->SetRenderLayerName("Default");

		BreathSprite->AddAnimationSequence(AnimationName, true);

		BreathSprite->AttachToComponent(GetRoot());

		BreathSprite->SetAnimationFlip(true);

		_BreathSprites.push_back(BreathSprite);
	}

	SetPatternEnable(false);

	return true;
}

void Boss2Dragon::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	if (_Appearing)
	{
		_ElapsedTime += DeltaTime;

		float Ratio = _ElapsedTime / _PatternData.AppearanceTime;

		if (Ratio > 1.f)
		{
			Ratio = 1.f;
		}

		FVector3D Position = _PatternData.CreatePosition;

		Position._x += (_PatternData.LowerPosition._x - _PatternData.CreatePosition._x) * Ratio;

		Position._y += (_PatternData.LowerPosition._y - _PatternData.CreatePosition._y) * Ratio;

		SetWorldPosition(Position);

		if (Ratio < 1.f)
		{
			return;
		}

		SetWorldPosition(_PatternData.LowerPosition);

		_ElapsedTime = 0.f;

		_Appearing = false;

		_Preparing = true;

		_BodySprite->ChangeAnimation("Dragon.phase2.action.1.intro");

		_BodySprite->SetAnimationFrame(0);

		_BodySprite->SetPlay("Dragon.phase2.action.1.intro", true);

		return;
	}

	if (_Preparing)
	{
		return;
	}

	if (_Breathing)
	{
		// 몸체의 첫 브레스 동작을 끝까지 재생한 뒤 반복 모션으로 전환한다.
		Ptr<Animation2D> Animation = _BodySprite->GetAnimation();

		if (Animation && Animation->IsFinished())
		{
			_BodySprite->ChangeAnimation("Dragon.phase2.action.1.breathLoop");

			_BodySprite->SetAnimationFrame(0);

			_BodySprite->SetPlay("Dragon.phase2.action.1.breathLoop", true);
		}

		_ElapsedTime += DeltaTime;

		if (_ElapsedTime < _PatternData.SweepTime)
		{
			float Ratio = _ElapsedTime / _PatternData.SweepTime;

			FVector3D Position = _PatternData.LowerPosition;

			Position._x += (_PatternData.UpperPosition._x - _PatternData.LowerPosition._x) * Ratio;

			Position._y += (_PatternData.UpperPosition._y - _PatternData.LowerPosition._y) * Ratio;

			SetWorldPosition(Position);

			return;
		}

		if (_ElapsedTime < _PatternData.SweepTime * 2.f)
		{
			float Ratio = (_ElapsedTime - _PatternData.SweepTime) / _PatternData.SweepTime;

			FVector3D Position = _PatternData.UpperPosition;

			Position._x += (_PatternData.LowerPosition._x - _PatternData.UpperPosition._x) * Ratio;

			Position._y += (_PatternData.LowerPosition._y - _PatternData.UpperPosition._y) * Ratio;

			SetWorldPosition(Position);

			return;
		}

		SetWorldPosition(_PatternData.LowerPosition);

		_ElapsedTime = 0.f;

		_Breathing = false;

		_Ending = true;

		for (Ptr<SpriteComponent>& BreathSprite : _BreathSprites)
		{
			if (BreathSprite)
			{
				BreathSprite->SetEnable(false);
			}
		}

		_BodySprite->ChangeAnimation("Dragon.phase2.action.1.tail");

		_BodySprite->SetAnimationFrame(0);

		_BodySprite->SetPlay("Dragon.phase2.action.1.tail", true);

		return;
	}

	if (!_Ending)
	{
		return;
	}

	Ptr<Animation2D> Animation = _BodySprite->GetAnimation();

	if (!Animation || !Animation->IsFinished())
	{
		return;
	}

	SetPatternEnable(false);
}

void Boss2Dragon::Destroy()
{
	_BreathSprites.clear();

	_BodySprite.reset();

	Actor::Destroy();
}

void Boss2Dragon::Start()
{
	_ElapsedTime = 0.f;

	_Appearing = true;

	_Preparing = false;

	_Breathing = false;

	_Ending = false;

	SetWorldPosition(_PatternData.CreatePosition);

	SetPatternEnable(true);

	_BodySprite->ChangeAnimation("Dragon.phase2.action.0");

	_BodySprite->SetAnimationFrame(0);

	_BodySprite->SetPlay("Dragon.phase2.action.0", true);
}

void Boss2Dragon::SetPatternEnable(bool Enable)
{
	SetEnable(Enable);

	if (_BodySprite)
	{
		_BodySprite->SetEnable(Enable);
	}

	for (Ptr<SpriteComponent>& BreathSprite : _BreathSprites)
	{
		if (BreathSprite)
		{
			BreathSprite->SetEnable(false);
		}
	}

	if (!Enable)
	{
		_ElapsedTime = 0.f;

		_Appearing = false;

		_Preparing = false;
		
		_Breathing = false;

		_Ending = false;
	}
}
