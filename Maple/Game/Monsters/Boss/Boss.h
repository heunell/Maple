#pragma once
#include "Game/Monsters/MonsterBase.h"


class Boss : public MonsterBase
{
public:
	Boss() = default;
	virtual ~Boss() = default;
	Boss(const Boss&) = delete;
	Boss(Boss&&) = delete;
	Boss& operator=(const Boss&) = delete;
	Boss& operator=(Boss&&) = delete;


private:
	Ptr<class SpriteComponent> _BottomSprite;

	Ptr<class SpriteComponent> _BossSprite;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
	
	Ptr<class SpriteComponent> GetBossSprite() const;
};