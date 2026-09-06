#include "pch.h"
#include "BossPhase1Golem.h"
#include "BossPhase1GolemState.h"
#include "Core/GameEngine.h"
#include "Collision/CollisionProfile.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "World/World.h"

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

	_Collision = CreateSceneComponent<AABBCollisionComponent>("GolemCollision");

	if (!_Collision)
	{
		return false;
	}

	_Collision->SetBoxSize(145.f, 240.f);

	_Collision->SetRelativePosition(-1.5f, 100.5f, 0.f);
	
	_Collision->AttachToComponent(GetRoot());

	_Collision->SetCollisionProfile("Monster");

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

void BossPhase1Golem::Collision(float DeltaTime)
{
	MonsterBase::Collision(DeltaTime);

	if (!IsStanding() || !_Collision || !_Collision->GetProfile())
	{
		return;
	}

	if (_Collision->GetProfile()->GetName() == "Golem")
	{
		return;
	}

	Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

	if (!CurrentWorld)
	{
		return;
	}

	Ptr<Actor> Player = CurrentWorld->GetPlayer();

	if (!Player || !Player->IsActive() || !Player->IsEnable())
	{
		return;
	}

	Ptr<AABBCollisionComponent> PlayerCollision = Player->FindSceneComponent<AABBCollisionComponent>("AABB");

	if (!PlayerCollision || !PlayerCollision->IsActive() || !PlayerCollision->IsEnable())
	{
		return;
	}

	std::pair<int32, int32> PlayerCollisionID = PlayerCollision->GetColliderID();

	if (_Collision->CheckState(PlayerCollisionID) != COLLISION_STATE_RELEASE)
	{
		return;
	}

	_Collision->SetCollisionProfile("Golem");
}

void BossPhase1Golem::Start(Ptr<BossPhase1GolemState> Owner, const FVector3D& Position, float GroundY, float FallDuration)
{
	_Owner = Owner;

	_Collision->SetCollisionProfile("Monster");

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

bool BossPhase1Golem::IsStanding() const
{
    return IsEnable() && !_Summoning && !_Falling && !_Landing;
}

void BossPhase1Golem::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (_Collision)
	{
		_Collision->SetEnable(Enable);
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