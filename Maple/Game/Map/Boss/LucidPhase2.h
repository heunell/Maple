#pragma once
#include "Object/Actor.h"
#include "Component/AABBCollisionComponent.h"
#include <vector>

struct FPlatformCollisionData
{
	const char* Name;

	FVector3D Position;

	FVector2D Size;
};

struct FBackgroundPart
{
	std::string Name;

	Ptr<SceneComponent> Parent;
};

struct FBackgroundScrollLayer
{
	Weak<class SceneComponent> Root;

	float ScrollSpeed = 0.f;
};

class LucidPhase2 : public Actor
{
public:
	LucidPhase2() = default;
	virtual ~LucidPhase2() = default;
	LucidPhase2(const LucidPhase2&) = delete;
	LucidPhase2(LucidPhase2&&) = delete;
	LucidPhase2& operator=(const LucidPhase2&) = delete;
	LucidPhase2& operator=(LucidPhase2&&) = delete;

private:
	Ptr<AABBCollisionComponent> _LeftWall;

	Ptr<AABBCollisionComponent> _RightWall;

	std::vector<FBackgroundScrollLayer> _BackgroundScrollLayers;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;
};