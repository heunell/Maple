#include "pch.h"
#include "BossMushroom.h"
#include "BossMushroomState.h"
#include "Component/SpriteComponent.h"

bool BossMushroom::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	_Status.MaxHP = 20000000;

	_Status.CurrentHP = _Status.MaxHP;

	_Sprite = CreateSceneComponent<SpriteComponent>("BossMushroomSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("LUCID_MUSHROOM_8880157.regen", false);

	_Sprite->AddAnimationSequence("LUCID_MUSHROOM_8880157.move", true);

	_Sprite->AddAnimationSequence("LUCID_MUSHROOM_8880157.hit1", false);

	_Sprite->AddAnimationSequence("LUCID_MUSHROOM_8880157.die1", false);

	_Sprite->AttachToComponent(GetRoot());

	return true;
}

void BossMushroom::Tick(float DeltaTime)
{
	MonsterBase::Tick(DeltaTime);

	Ptr<Animation2D> Animation = _Sprite->GetAnimation();

	if (!_Moving)
	{
		if (!Animation || !Animation->IsFinished())
		{
			return;
		}

		if (_Dying)
		{
			Ptr<BossMushroomState> Owner = Lock(_Owner);

			if (!Owner)
			{
				SetPoolEnable(false);

				return;
			}

			Owner->ReleaseMushroom(This<BossMushroom>());

			return;
		}

		_Moving = true;

		_Sprite->ChangeAnimation("LUCID_MUSHROOM_8880157.move");

		return;
	}

	AddWorldPosition(_MoveSpeed * static_cast<float>(_Direction) * DeltaTime, 0.f, 0.f);

	float CurrentX = GetWorldPosition()._x;

	if ((_Direction < 0 && CurrentX > _EndX) || (_Direction > 0 && CurrentX < _EndX))
	{
		return;
	}

	_Moving = false;

	_Dying = true;

	_Sprite->ChangeAnimation("LUCID_MUSHROOM_8880157.die1");
}

void BossMushroom::Start(Ptr<BossMushroomState> Owner, const FVector3D& Position, int32 Direction, float EndX)
{
	_Owner = Owner;

	_Direction = Direction < 0 ? -1 : 1;

	_EndX = EndX;

	_Moving = false;

	_Dying = false;

	_Status.CurrentHP = _Status.MaxHP;

	SetWorldPosition(Position);

	// 원본 버섯은 왼쪽을 바라보므로 오른쪽 이동 버섯만 뒤집는다.
	_Sprite->SetAnimationFlip(_Direction > 0);

	_Sprite->ChangeAnimation("LUCID_MUSHROOM_8880157.regen");
}

void BossMushroom::SetPoolEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		_Owner.reset();

		_Direction = 0;

		_EndX = 0.f;

		_Moving = false;

		_Dying = false;
	}
}