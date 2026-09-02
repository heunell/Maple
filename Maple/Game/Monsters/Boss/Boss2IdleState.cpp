#include "pch.h"
#include "Boss2IdleState.h"
#include "Boss2BlackBoard.h"
#include "Boss2Component.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"


void Boss2IdleState::Enter(Ptr<MonsterComponent> Monster)
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

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (BossSprite)
	{
		BossSprite->ChangeAnimation("LUCID_MOB_8880150.fly");
	}
}

Ptr<MonsterState> Boss2IdleState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
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

	Ptr<Boss2BlackBoard> BossBoard = Cast<MonsterBlackBoard, Boss2BlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return nullptr;
	}

	_ElapsedTime += DeltaTime;

	if (_ElapsedTime < BossBoard->IdleDuration)
	{
		return nullptr;
	}

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	Ptr<MonsterState> PatternState = BossController->SelectPatternState();

	if (!PatternState)
	{
		_ElapsedTime = 0.f;

		return nullptr;
	}

	return PatternState;
}