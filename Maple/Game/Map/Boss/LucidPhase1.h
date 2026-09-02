#pragma once
#include "Object/Actor.h"
#include "Component/AABBCollisionComponent.h"

class LucidPhase1 : public Actor
{
public:
	LucidPhase1() = default;
	virtual ~LucidPhase1() = default;

	LucidPhase1(const LucidPhase1&) = delete;
	LucidPhase1(LucidPhase1&&) = delete;
	LucidPhase1& operator=(const LucidPhase1&) = delete;
	LucidPhase1& operator=(LucidPhase1&&) = delete;

private:
	Ptr<AABBCollisionComponent> _Clamp;

	Ptr<AABBCollisionComponent> _LeftWall;

	Ptr<AABBCollisionComponent> _RightWall;
	
	Ptr<AABBCollisionComponent> _Floor;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

private:
	bool InitBossPatternArea(Ptr<class Boss> Boss);
};