#include "pch.h"
#include "Boss2Dragon.h"
#include "Component/SpriteComponent.h"

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

	_BodySprite->AddNotify("Dragon.phase2.action.1.intro", _PatternData.BreathStartFrame, this, &Boss2Dragon::StartBreathPattern);

	_BreathSprite = CreateSceneComponent<SpriteComponent>("Boss2DragonBreath");

	if (!_BreathSprite)
	{
		return false;
	}

	_BreathSprite->SetRenderLayerName("Default");

	_BreathSprite->AddAnimationSequence("Dragon.phase2.breath.tile.0", true);

	_BreathSprite->AttachToComponent(GetRoot());

	SetPatternEnable(false);

	return true;
}

void Boss2Dragon::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	if (_Appearing)
	{
		_ElapsedTime += DeltaTime;

		if (_ElapsedTime < _PatternData.AppearanceTime)
		{
			return;
		}

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
		_ElapsedTime += DeltaTime;

		if (_ElapsedTime < _PatternData.BreathTime)
		{
			return;
		}

		_ElapsedTime = 0.f;

		_Breathing = false;

		_Ending = true;

		_BreathSprite->SetEnable(false);

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

void Boss2Dragon::StartBreathPattern()
{
	if (!_Preparing)
	{
		return;
	}

	_Preparing = false;

	_Breathing = true;

	_ElapsedTime = 0.f;

	_BodySprite->ChangeAnimation("Dragon.phase2.action.1.breathLoop");

	_BodySprite->SetAnimationFrame(0);

	_BodySprite->SetPlay("Dragon.phase2.action.1.breathLoop", true);

	_BreathSprite->SetEnable(true);

	_BreathSprite->SetAnimationFrame(0);

	_BreathSprite->SetPlay("Dragon.phase2.breath.tile.0", true);
}

void Boss2Dragon::Start(const FVector3D& Position)
{
	_ElapsedTime = 0.f;

	_Appearing = true;

	_Preparing = false;

	_Breathing = false;

	_Ending = false;

	SetWorldPosition(Position);

	SetPatternEnable(true);

	_BodySprite->ChangeAnimation("Dragon.phase2.action.0");

	_BodySprite->SetAnimationFrame(0);

	_BodySprite->SetPlay("Dragon.phase2.action.0", true);

	_BreathSprite->SetEnable(false);
}

void Boss2Dragon::SetPatternEnable(bool Enable)
{
	SetEnable(Enable);

	if (_BodySprite)
	{
		_BodySprite->SetEnable(Enable);
	}

	if (_BreathSprite)
	{
		_BreathSprite->SetEnable(false);
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

void Boss2Dragon::Destroy()
{
	_BreathSprite.reset();

	_BodySprite.reset();

	Actor::Destroy();
}