#pragma once
#include "Core/Object.h"

class MonsterBlackBoard : public Object
{
public:
	MonsterBlackBoard() = default;
	virtual ~MonsterBlackBoard() = default;
	MonsterBlackBoard(const MonsterBlackBoard&) = delete;
	MonsterBlackBoard(MonsterBlackBoard&&) = delete;
	MonsterBlackBoard& operator=(const MonsterBlackBoard&) = delete;
	MonsterBlackBoard& operator=(MonsterBlackBoard&&) = delete;

public:
	virtual void Destroy() override;
};