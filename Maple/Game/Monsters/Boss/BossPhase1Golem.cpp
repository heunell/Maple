#include "pch.h"
#include "BossPhase1Golem.h"
#include "BossPhase1GolemState.h"
#include "Component/SpriteComponent.h"

bool BossPhase1Golem::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	_Status.MaxHP = 35000000;

	_Status.CurrentHP = _Status.MaxHP;

	_Sprite = CreateSceneComponent<SpriteComponent>("BossPhase1GolemSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880160.attack1", false);

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880161.revive", false);

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880161.stand", true);

	_Sprite->AttachToComponent(GetRoot());

	return true;
}

void BossPhase1Golem::Tick(float DeltaTime)
{
	MonsterBase::Tick(DeltaTime);

	if (!_Sprite)
	{
		return;
	}

	Ptr<Animation2D> Animation = _Sprite->GetAnimation();

	if (!Animation)
	{
		return;
	}

	if (_Summoning)
	{
		if (!Animation->IsFinished())
		{
			return;
		}

		_Summoning = false;

		_Falling = true;

		_FallData.ElapsedTime = 0.f;

		return;
	}

	if (_Falling)
	{
		_FallData.ElapsedTime += DeltaTime;

		float FallRatio = 1.f;

		if (_FallData.Duration > 0.f)
		{
			FallRatio = _FallData.ElapsedTime / _FallData.Duration;
		}

		if (FallRatio > 1.f)
		{
			FallRatio = 1.f;
		}

		FVector3D Position = GetWorldPosition();

		Position._y = _FallData.StartY + (_FallData.GroundY - _FallData.StartY) * FallRatio;

		SetWorldPosition(Position);

		if (FallRatio < 1.f)
		{
			return;
		}

		_Falling = false;

		_Landing = true;

		_Sprite->ChangeAnimation("LUCID_GOLEM_8880161.revive");

		_Sprite->SetAnimationFrame(0);

		_Sprite->SetPlay("LUCID_GOLEM_8880161.revive", true);

		return;
	}

	if (!_Landing)
	{
		return;
	}

	if (!Animation->IsFinished())
	{
		return;
	}

	_Landing = false;

	_Sprite->ChangeAnimation("LUCID_GOLEM_8880161.stand");

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay("LUCID_GOLEM_8880161.stand", true);
}

void BossPhase1Golem::Start(Ptr<BossPhase1GolemState> Owner, const FVector3D& Position, float GroundY, float FallDuration)
{
	_Owner = Owner;

	_FallData.StartY = Position._y;

	_FallData.GroundY = GroundY;

	_FallData.Duration = FallDuration;

	_FallData.ElapsedTime = 0.f;

	_Summoning = true;

    _Falling   = false;

    _Landing   = false;

	_Status.CurrentHP = _Status.MaxHP;

	SetWorldPosition(Position);

	_Sprite->ChangeAnimation("LUCID_GOLEM_8880160.attack1");

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay("LUCID_GOLEM_8880160.attack1", true);
}

void BossPhase1Golem::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		_Owner.reset();

		_FallData = FBossPhase1GolemFallData();

		_Summoning = false;

        _Falling = false;

        _Landing = false;
	}
}