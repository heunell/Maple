#include "pch.h"
#include "BossFlower.h"
#include "BossFlowerState.h"
#include "Component/SpriteComponent.h"
#include "Component/OBBCollisionComponent.h"

bool BossFlower::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("BossAttack");

	_Sprite = CreateSceneComponent<SpriteComponent>("BossFlowerSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("lucid_flower_ms", false);

	_Sprite->AddAnimationSequence("lucid_flower_m", false);

	_Sprite->AddAnimationSequence("lucid_flower_l", false);

	_Sprite->AddAnimationSequence("lucid_flower_xl", false);

	_Sprite->AttachToComponent(GetRoot());

	_Collision = CreateSceneComponent<OBBCollisionComponent>("FlowerCollision");

	if (!_Collision)
	{
		return false;
	}

	// Sprite와 같은 Root의 회전을 상속한다.
	_Collision->AttachToComponent(GetRoot());

	_Collision->SetCollisionProfile("BossAttack");
	
	_Collision->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &BossFlower::OnOverlap);

	_Collision->SetEnable(false);

	return true;
}

void BossFlower::Tick(float DeltaTime)
{
	_ElapsedTime += DeltaTime;

	Actor::Tick(DeltaTime);

	Ptr<Animation2D> Animation = _Sprite->GetAnimation();

	if (!Animation || !Animation->IsFinished())
	{
		return;
	}

	Ptr<BossFlowerState> Owner = Lock(_Owner);

	if (!Owner)
	{
		SetPoolEnable(false);
		return;
	}

	Owner->ReleaseFlower(This<BossFlower>());
}

void BossFlower::Start(Ptr<class BossFlowerState> Owner, const FVector3D& Position, int32 ResourceIndex, float Rotation)
{
	_Owner = Owner;

	_ElapsedTime = 0.f;

	_HitData.Hit = false;

	_Collision->SetEnable(true);
	
	SetWorldPosition(Position);

	SetRelativeRotation(0.f, 0.f, Rotation);

	std::string AnimationName;

	switch (ResourceIndex)
	{
	case 0:
		AnimationName = "lucid_flower_ms";
		_Collision->SetBoxSize(25.f, 335.f);
		_Collision->SetRelativePosition(0.f, 192.5f, 0.f);
		break;

	case 1:
		AnimationName = "lucid_flower_m";
		_Collision->SetBoxSize(40.f, 423.f);
		_Collision->SetRelativePosition(0.f, 248.f, 0.f);
		break;

	case 2:
		AnimationName = "lucid_flower_l";
		_Collision->SetBoxSize(57.f, 536.f);
		_Collision->SetRelativePosition(0.f, 340.f, 0.f);
		break;

	case 3:
		AnimationName = "lucid_flower_xl";
		_Collision->SetBoxSize(74.f, 528.f);
		_Collision->SetRelativePosition(0.f, 408.f, 0.f);
		break;
	}

	_Sprite->ChangeAnimation(AnimationName);

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay(AnimationName, true);
}

void BossFlower::SetPoolEnable(bool Enable)
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

		_ElapsedTime = 0.f;
	}
}

void BossFlower::OnOverlap(Weak<CollisionComponent> Destination)
{
	if (!_Collision || !_Collision->IsEnable())
	{
		return;
	}

	if (_HitData.Hit || _ElapsedTime < _HitData.StartTime || _ElapsedTime >= _HitData.StartTime + _HitData.Duration)
	{
		return;
	}

	Ptr<CollisionComponent> HitCollision = Lock<CollisionComponent>(Destination);

	if (!HitCollision)
	{
		return;
	}

	Ptr<Actor> Player = Lock<Actor>(HitCollision->GetOwner());

	if (!Player)
	{
		return;
	}

	if (HitCollision != Player->FindSceneComponent<CollisionComponent>("AABB"))
	{
		return;
	}

	_HitData.Hit = true;

	// Todo : Player 피해처리
}