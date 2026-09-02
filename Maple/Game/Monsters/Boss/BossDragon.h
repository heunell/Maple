#pragma once

#include "Object/Actor.h"
#include "BossPatternData.h"

#include <vector>

class BossDragon : public Actor
{
public:
	BossDragon() = default;
	virtual ~BossDragon() = default;
	BossDragon(const BossDragon&) = delete;
	BossDragon(BossDragon&&) = delete;
	BossDragon& operator=(const BossDragon&) = delete;
	BossDragon& operator=(BossDragon&&) = delete;

private:
	Ptr<class SpriteComponent> _BodySprite;

	std::vector<Ptr<class SpriteComponent>> _BreathSprites;

	FBossDragonPatternData _PatternData;

	FVector3D _StartPosition = FVector3D::Zero;

	FVector3D _DragonPosition = FVector3D::Zero;

	float _ElapsedTime = 0.f;

	bool _Appearing = false;

	bool _Preparing = false;

	bool _Breathing = false;

	bool _Ending = false;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void Start(const FVector3D& StartPosition, const FVector3D& DragonPosition, bool LeftSide);

	void SetPatternEnable(bool Enable);
};