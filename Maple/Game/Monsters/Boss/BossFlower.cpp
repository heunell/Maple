#include "pch.h"
#include "BossFlower.h"
#include "BossFlowerState.h"
#include "Component/SpriteComponent.h"

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

	return true;
}

void BossFlower::Tick(float DeltaTime)
{
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

	SetWorldPosition(Position);

	SetRelativeRotation(0.f, 0.f, Rotation);

	switch (ResourceIndex)
	{
	case 0:
		_Sprite->ChangeAnimation("lucid_flower_ms");
		break;

	case 1:
		_Sprite->ChangeAnimation("lucid_flower_m");
		break;

	case 2:
		_Sprite->ChangeAnimation("lucid_flower_l");
		break;

	case 3:
		_Sprite->ChangeAnimation("lucid_flower_xl");
		break;
	}
}

void BossFlower::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		_Owner.reset();
	}
}
