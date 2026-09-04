#pragma once
#include "Game/Monsters/MonsterState.h"
#include "Core/ObjectPool.h"
#include "BossPatternData.h"
#include "Boss2Laser.h"

class Boss2LaserState : public MonsterState
{
public:
	Boss2LaserState() = default;
	virtual ~Boss2LaserState() = default;
	Boss2LaserState(const Boss2LaserState&) = delete;
	Boss2LaserState(Boss2LaserState&&) = delete;
	Boss2LaserState& operator=(const Boss2LaserState&) = delete;
	Boss2LaserState& operator=(Boss2LaserState&&) = delete;


};