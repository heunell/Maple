#pragma once
#include "Object/Actor.h"

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
    Ptr<class SpriteComponent> _Sprite;

    Weak<class Boss2LaserState> _Owner;

    float _ElapsedTime = 0.f;   // 생명주기로 봐야하나?

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class Boss2LaserState> Owner, const FVector3D& Position, float Rotation);

	void SetPoolEnable(bool Enable);
};

