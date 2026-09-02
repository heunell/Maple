#pragma once

#include "Object/Actor.h"

// MovementComponent에서 기능구현을 해놓고 해당 클래스로 이펙트만 입힌다.

enum class eWindJumpType
{
	DoubleFront,
	DoubleBack,
	UpFront,
	UpBack,
	UpSpecial,
	End
};

class WindJump : public Actor
{
public:
	WindJump() = default;
	virtual ~WindJump() = default;

private:
	Ptr<class SpriteComponent> _Sprite;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(eWindJumpType Type, bool IsRight);
};

