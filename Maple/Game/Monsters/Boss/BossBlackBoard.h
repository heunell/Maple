#pragma once
#include "Game/Monsters/MonsterBlackBoard.h"

class BossBlackBoard : public MonsterBlackBoard
{
public:
	BossBlackBoard() = default;
	virtual ~BossBlackBoard() = default;
	BossBlackBoard(const BossBlackBoard&) = delete;
	BossBlackBoard(BossBlackBoard&&) = delete;
	BossBlackBoard& operator=(const BossBlackBoard&) = delete;
	BossBlackBoard& operator=(BossBlackBoard&&) = delete;

public:
	float IdleDuration = 4.f;
};