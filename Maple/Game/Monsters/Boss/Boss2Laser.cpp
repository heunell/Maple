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

	_Sprite = CreateSceneComponent<SpriteComponent>("Boss2LaserSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("LaserRain.laser", false);

	_Sprite->AttachToComponent(GetRoot());

	// 프레임 진행은 Tick에서 경과 시간을 기준으로 제어한다.
	_Sprite->SetPlay("LaserRain.laser", false);

	SetPoolEnable(false);

	return true;
}

void Boss2Laser::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	_ElapsedTime += DeltaTime;

	if (_ElapsedTime < _PatternData.WarningTime)
	{
		int32 Frame = static_cast<int32>(_ElapsedTime / _PatternData.WarningFrameDelay);

		Frame %= _PatternData.WarningEndFrame - _PatternData.WarningStartFrame + 1;

		_Sprite->SetAnimationFrame(_PatternData.WarningStartFrame + Frame);

		return;
	}

	// 현재는 충돌 없이 Hit 프레임만 표시한다.
	if (_ElapsedTime < _PatternData.WarningTime + _PatternData.HitTime)
	{
		_Sprite->SetAnimationFrame(_PatternData.HitFrame);

		return;
	}

	int32 Frame = _PatternData.FadeStartFrame + static_cast<int32>(
		(_ElapsedTime - _PatternData.WarningTime - _PatternData.HitTime) / _PatternData.FadeFrameDelay);

	if (Frame <= _PatternData.FadeEndFrame)
	{
		_Sprite->SetAnimationFrame(Frame);

		return;
	}

	Ptr<Boss2LaserState> Owner = Lock(_Owner);

	if (!Owner)
	{
		SetPoolEnable(false);

		return;
	}

	Owner->ReleaseLaser(This<Boss2Laser>());
}

void Boss2Laser::Start(Ptr<Boss2LaserState> Owner, const FVector3D& Position, float Rotation)
{
	_Owner = Owner;

	_ElapsedTime = 0.f;

	SetWorldPosition(Position);

	SetRelativeRotation(0.f, 0.f, Rotation);

	_Sprite->ChangeAnimation("LaserRain.laser");

	_Sprite->SetPlay("LaserRain.laser", false);

	_Sprite->SetAnimationFrame(_PatternData.WarningStartFrame);
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