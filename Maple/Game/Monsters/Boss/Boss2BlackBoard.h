#pragma once
#include "Game/Monsters/MonsterBlackBoard.h"


class Boss2BlackBoard : public MonsterBlackBoard
{
public:
	Boss2BlackBoard() = default;
	virtual ~Boss2BlackBoard() = default;
	Boss2BlackBoard(const Boss2BlackBoard&) = delete;
	Boss2BlackBoard(Boss2BlackBoard&&) = delete;
	Boss2BlackBoard& operator=(const Boss2BlackBoard&) = delete;
	Boss2BlackBoard& operator=(Boss2BlackBoard&&) = delete;

public:
	float IdleDuration = 4.f;
};