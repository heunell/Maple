#pragma once
#include "Game/Monsters/MonsterBase.h"

class Boss2 : public MonsterBase
{
public:
	Boss2() = default;

	virtual ~Boss2() = default;

	Boss2(const Boss2&) = delete;

	Boss2(Boss2&&) = delete;

	Boss2& operator=(const Boss2&) = delete;

	Boss2& operator=(Boss2&&) = delete;


private:
	Ptr<class SpriteComponent> _BossSprite;


public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	Ptr<class SpriteComponent> GetBossSprite() const;
};