#pragma once
#include "Object/Actor.h"
#include "BossPatternData.h"

class Boss2Laser : public Actor
{
public:
	Boss2Laser() = default;
	virtual ~Boss2Laser() = default;
	Boss2Laser(const Boss2Laser&) = delete;
	Boss2Laser(Boss2Laser&&) = delete;
	Boss2Laser& operator=(const Boss2Laser&) = delete;
	Boss2Laser& operator=(Boss2Laser&&) = delete;

private:
	Ptr<class SpriteComponent>  _Sprite;

	Weak<class Boss2LaserState> _Owner;

	FBoss2LaserData				_LaserData;

	float _ElapsedTime = 0.f;

private:
	void ReturnToPool();

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void Start(Ptr<class Boss2LaserState> Owner, const FVector3D& Position, float Rotation);

	void SetPoolEnable(bool Enable);
};