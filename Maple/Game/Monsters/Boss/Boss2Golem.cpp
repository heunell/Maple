#include "pch.h"
#include "Boss2Golem.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Game/Map/Boss/LucidPhase2.h"

bool Boss2Golem::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	_Status.MaxHP = _PatternData.MaxHP;

	_Status.CurrentHP = _Status.MaxHP;

	_Sprite = CreateSceneComponent<SpriteComponent>("Boss2GolemSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880170.attack1", false);

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880171.regen", false);

	_Sprite->AddAnimationSequence("LUCID_GOLEM_8880171.stand", true);

	_Sprite->AttachToComponent(GetRoot());

	_Collision = CreateSceneComponent<AABBCollisionComponent>("GolemCollision");

	if (!_Collision)
	{
		return false;
	}

	_Collision->SetBoxSize(_PatternData.CollisionSize);

	_Collision->SetRelativePosition(_PatternData.CollisionOffset);

	_Collision->AttachToComponent(GetRoot());

	_Collision->SetCollisionProfile("Monster");

	SetPoolEnable(false);

	return true;
}

void Boss2Golem::Tick(float DeltaTime)
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

		_ElapsedTime = 0.f;

		_Sprite->ChangeAnimation("LUCID_GOLEM_8880171.regen");

		_Sprite->SetAnimationFrame(0);

		_Sprite->SetPlay("LUCID_GOLEM_8880171.regen", true);

		return;
	}

	if (!_Falling)
	{
		return;
	}

	_ElapsedTime += DeltaTime;

	float FallRatio = 1.f;

	if (_PatternData.FallDuration > 0.f)
	{
		FallRatio = _ElapsedTime / _PatternData.FallDuration;
	}

	if (FallRatio > 1.f)
	{
		FallRatio = 1.f;
	}

	// 이전 구현처럼 낙하 후반으로 갈수록 빠르게 이동한다.
	FallRatio *= FallRatio;

	FVector3D Position = _LandingPosition;

	Position._y += _PatternData.SpawnHeight * (1.f - FallRatio);

	SetWorldPosition(Position);

	if (FallRatio < 1.f || !Animation->IsFinished())
	{
		return;
	}

	SetWorldPosition(_LandingPosition);

	_Falling = false;

	Ptr<LucidPhase2> Map = Lock(_Map);

	if (Map)
	{
		Map->BreakPlatform(_PlatformIndex);
	}

	_Sprite->ChangeAnimation("LUCID_GOLEM_8880171.stand");

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay("LUCID_GOLEM_8880171.stand", true);

	_Collision->SetEnable(true);
}

void Boss2Golem::Start(Ptr<Boss2GolemState> Owner, Ptr<LucidPhase2> Map, int32 PlatformIndex, const FVector3D& LandingPosition)
{
	_Owner = Owner;

	_Map = Map;

	_PlatformIndex = PlatformIndex;

	_LandingPosition = LandingPosition;

	_ElapsedTime = 0.f;

	_Summoning = true;

	_Falling = false;

	_Status.CurrentHP = _Status.MaxHP;

	FVector3D Position = _LandingPosition;

	Position._y += _PatternData.SpawnHeight;

	SetWorldPosition(Position);

	_Collision->SetEnable(false);

	_Sprite->ChangeAnimation("LUCID_GOLEM_8880170.attack1");

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay("LUCID_GOLEM_8880170.attack1", true);
}

void Boss2Golem::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (_Collision)
	{
		_Collision->SetEnable(false);
	}

	if (!Enable)
	{
		_Owner.reset();

		_Map.reset();

		_PlatformIndex = -1;

		_LandingPosition = FVector3D::Zero;

		_ElapsedTime = 0.f;

		_Summoning = false;

		_Falling = false;
	}
}