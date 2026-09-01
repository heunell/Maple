#include "pch.h"
#include "BossIdleState.h"
#include "Boss.h"
#include "BossBlackBoard.h"
#include "BossComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterComponent.h"

void BossIdleState::Enter(Ptr<class MonsterComponent> Monster)
{
    _ElapsedTime = 0.f;

    if (!Monster)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<Boss> BossOwner = Cast<MonsterBase, Boss>(MonsterOwner);

	if (!BossOwner)
	{
		return;
	}

	Ptr<SpriteComponent> BossSprite = BossOwner->GetBossSprite();

	if (!BossSprite)
	{
		return;
	}

	BossSprite->ChangeAnimation("LUCID_MOB_8880140.stand");
}

Ptr<MonsterState> BossIdleState::Tick(Ptr<class MonsterComponent> Monster, float DeltaTime)
{
    if (!Monster)
	{
		return nullptr;
	}

	Ptr<MonsterBlackBoard> MonsterBoard = Monster->GetBlackBoard();

	if (!MonsterBoard)
	{
		return nullptr;
	}

	Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return nullptr;
	}

	_ElapsedTime += DeltaTime;

	if (_ElapsedTime < BossBoard->IdleDuration)
	{
		return nullptr;
	}

	Ptr<BossComponent> Boss = Cast<MonsterComponent, BossComponent>(Monster);

	if (!Boss)
	{
		return nullptr;
	}

	Ptr<MonsterState> PatternState = Boss->SelectPatternState();

	if (!PatternState)
	{
		_ElapsedTime = 0.f;

		return nullptr;
	}

	return PatternState;
}
