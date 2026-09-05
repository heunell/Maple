#pragma once

#include "Object/Actor.h"
#include "BossPatternData.h"
#include <vector>

class Boss2Dragon : public Actor
{
public:
	Boss2Dragon() = default;
	virtual ~Boss2Dragon() = default;
	Boss2Dragon(const Boss2Dragon&) = delete;
	Boss2Dragon(Boss2Dragon&&) = delete;
	Boss2Dragon& operator=(const Boss2Dragon&) = delete;
	Boss2Dragon& operator=(Boss2Dragon&&) = delete;

private:
	Ptr<class SpriteComponent> _BodySprite;

	std::vector<Ptr<class SpriteComponent>> _BreathSprites;

	FBoss2DragonPatternData _PatternData;

	float _ElapsedTime = 0.f;

	bool _Appearing = false;

	bool _Preparing = false;

	bool _Breathing = false;

	bool _Ending = false;

private:
	void StartBreathPattern();

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void Start();

	void SetPatternEnable(bool Enable);
};