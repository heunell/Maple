#pragma once
#include "Object/Actor.h"
#include "Component/AABBCollisionComponent.h"

// 같은 배경에 다른 Actor를 배치하는 논리맵구조
enum class eRoomType
{
	Entry,		// 배치하는 Actor : NPC
	Reward,		// 배치하는 Actor : Gate(Portal), RewardBox(MonsterType)
	End
};

class LucidEntry : public Actor
{
public:
	LucidEntry();
	virtual ~LucidEntry();
	LucidEntry(const LucidEntry&) = delete;
	LucidEntry(LucidEntry&&) = delete;
	LucidEntry& operator = (const LucidEntry&) = delete;
	LucidEntry& operator = (LucidEntry&&) = delete;

private:
	Ptr<AABBCollisionComponent> _Clamp;

	Ptr<AABBCollisionComponent> _LeftWall;

	Ptr<AABBCollisionComponent> _RightWall;
	
	Ptr<AABBCollisionComponent> _Floor;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, eRoomType RoomType);

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;
};

