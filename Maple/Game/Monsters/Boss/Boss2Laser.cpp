#include "pch.h"
#include "Boss2Laser.h"
#include "Boss2LaserState.h"
#include "Component/SpriteComponent.h"

bool Boss2Laser::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("BossAttack");

	_Sprite = CreateSceneComponent<SpriteComponent>("Boss2LaserSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Monster");

	_Sprite->AddAnimationSequence("LaserRain.laser", false);

	_Sprite->AttachToComponent(GetRoot());

	SetPoolEnable(false);

	return true;
}

void Boss2Laser::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	_ElapsedTime += DeltaTime;

	if (_ElapsedTime < _LaserData.WarningTime)
	{
		int32 WarningFrameCount = _LaserData.WarningEndFrame - _LaserData.WarningStartFrame + 1;

		int32 Frame = _LaserData.WarningStartFrame + static_cast<int32>(_ElapsedTime / _LaserData.WarningFrameDelay) % WarningFrameCount;

		_Sprite->SetAnimationFrame(Frame);

		return;
	}

	float ActiveElapsedTime = _ElapsedTime - _LaserData.WarningTime;

	if (ActiveElapsedTime < _LaserData.ActiveTime)
	{
		_Sprite->SetAnimationFrame(_LaserData.ActiveFrame);

		// 충돌은 이후 이 구간에서만 활성화한다.
		return;
	}

	float FadeElapsedTime = ActiveElapsedTime - _LaserData.ActiveTime;

	int32 FadeFrame = _LaserData.ActiveFrame + 1 + static_cast<int32>(FadeElapsedTime / _LaserData.FadeFrameDelay);

	if (FadeFrame > _LaserData.LastFrame)
	{
		ReturnToPool();

		return;
	}

	_Sprite->SetAnimationFrame(FadeFrame);
}

void Boss2Laser::Start(Ptr<class Boss2LaserState> Owner, const FVector3D& Position, float Rotation)
{
	if (!Owner || !_Sprite)
	{
		return;
	}

	_Owner = Owner;

	_ElapsedTime = 0.f;

	SetWorldPosition(Position);

	SetRelativeRotation(0.f, 0.f, Rotation);

	_Sprite->ChangeAnimation("LaserRain.laser");

	// 경고 구간을 직접 반복하므로 자동 재생은 멈춘다.
	_Sprite->SetPlay("LaserRain.laser", false);

	_Sprite->SetAnimationFrame(_LaserData.WarningStartFrame);
}

void Boss2Laser::SetPoolEnable(bool Enable)
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

void Boss2Laser::ReturnToPool()
{
	Ptr<Boss2LaserState> Owner = Lock(_Owner);

	if (!Owner)
	{
		SetPoolEnable(false);

		return;
	}

	//Owner->ReleaseLaser(This<Boss2Laser>());
}

void Boss2Laser::Destroy()
{
	_Owner.reset();

	_Sprite.reset();

	Actor::Destroy();
}