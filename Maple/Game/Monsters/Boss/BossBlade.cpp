#include "pch.h"
#include "BossBlade.h"
#include "BossBladeState.h"
#include "Component/SpriteComponent.h"

#include <cmath>

bool BossBlade::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("BossAttack");

	_Sprite = CreateSceneComponent<SpriteComponent>("BossBladeSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("level.4.ball0", true);

	_Sprite->AddAnimationSequence("level.4.ball1", true);

	_Sprite->AddAnimationSequence("level.4.ball2", true);

	_Sprite->AttachToComponent(GetRoot());

	return true;
}

void BossBlade::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	_ElapsedTime += DeltaTime;

	AddWorldPosition(_Direction._x * _Speed * DeltaTime, _Direction._y * _Speed * DeltaTime, 0.f);

	if (_ElapsedTime < _LifeTime)
	{
		return;
	}

	Ptr<BossBladeState> Owner = Lock(_Owner);

	if (!Owner)
	{
		SetPoolEnable(false);

		return;
	}

	Owner->ReleaseBlade(This<BossBlade>());
}

void BossBlade::Start(Ptr<BossBladeState> Owner, const FVector3D& Position, const FVector3D& Direction, int32 ResourceIndex)
{
	_Owner = Owner;

	_Direction = Direction;

	_ElapsedTime = 0.f;

	SetWorldPosition(Position);

	float Rotation = DirectX::XMConvertToDegrees(atan2f(-_Direction._y, -_Direction._x));

	SetRelativeRotation(0.f, 0.f, Rotation);

	switch (ResourceIndex)
	{
	case 0:
		_Sprite->ChangeAnimation("level.4.ball0");
		break;

	case 1:
		_Sprite->ChangeAnimation("level.4.ball1");
		break;

	case 2:
		_Sprite->ChangeAnimation("level.4.ball2");
		break;
	}
}

void BossBlade::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		_Owner.reset();

		_ElapsedTime = 0.f;
	}
}