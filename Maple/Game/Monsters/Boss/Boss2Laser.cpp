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

	_Sprite->SetRenderLayerName(_LaserData.RenderLayer);

	_Sprite->AddAnimationSequence(_LaserData.Animation, false);

	_Sprite->AttachToComponent(GetRoot());

	SetPoolEnable(false);

	return true;
}

void Boss2Laser::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	if (!_Sprite)
	{
		return;
	}

	_ElapsedTime += DeltaTime;

	if (_ElapsedTime < _LaserData.WarningTime)
	{
		int32 WarningFrameCount = _LaserData.WarningEndFrame - _LaserData.WarningStartFrame + 1;

		int32 Frame = _LaserData.WarningStartFrame + static_cast<int32>(_ElapsedTime / _LaserData.WarningFrameDelay) % WarningFrameCount;

		_Sprite->SetOpacity(_LaserData.WarningOpacity);

		_Sprite->SetAnimationFrame(Frame);

		return;
	}

	float ActiveElapsedTime = _ElapsedTime - _LaserData.WarningTime;

	if (ActiveElapsedTime < _LaserData.ActiveTime)
	{
		_Sprite->SetOpacity(_LaserData.ActiveOpacity);

		_Sprite->SetAnimationFrame(_LaserData.ActiveFrame);

		// 충돌 구현 시 이 구간에서 레이저 하나당 한 번만 판정한다.
		return;
	}

	float FadeElapsedTime = ActiveElapsedTime - _LaserData.ActiveTime;

	int32 FadeFrame = _LaserData.ActiveFrame + 1 + static_cast<int32>(FadeElapsedTime / _LaserData.FadeFrameDelay);

	if (FadeFrame > _LaserData.LastFrame)
	{
		ReturnToPool();

		return;
	}

	_Sprite->SetOpacity(_LaserData.FadeOpacity);

	_Sprite->SetAnimationFrame(FadeFrame);
}

void Boss2Laser::Start(Ptr<Boss2LaserState> Owner, const FVector3D& Position, float Rotation)
{
	if (!Owner || !_Sprite)
	{
		return;
	}

	_Owner = Owner;

	_ElapsedTime = 0.f;

	SetWorldPosition(Position);

	SetRelativeRotation(0.f, 0.f, Rotation);

	_Sprite->ChangeAnimation(_LaserData.Animation);

	_Sprite->SetAnimationFrame(_LaserData.WarningStartFrame);

	// 경고 프레임 구간을 Tick에서 직접 반복한다.
	_Sprite->SetPlay(_LaserData.Animation, false);

	_Sprite->SetOpacity(_LaserData.WarningOpacity);
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

		if (_Sprite)
		{
			_Sprite->SetOpacity(1.f);
		}
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

	Owner->ReleaseLaser(This<Boss2Laser>());
}

void Boss2Laser::Destroy()
{
	_Owner.reset();

	_Sprite.reset();

	Actor::Destroy();
}