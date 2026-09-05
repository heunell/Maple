#include "pch.h"
#include "BossDragonState.h"
#include "BossBlackBoard.h"
#include "BossDragon.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "UI/BossPatternNotice.h"
#include "World/Level.h"

#include <random>

bool BossDragonState::Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState)
{
	if (!Owner || !IdleState)
	{
		return false;
	}

	_Owner = Owner;

	_IdleState = IdleState;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return false;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return false;
	}

	_Notice = LevelOwner->SpawnActor<BossPatternNotice>("BossDragonNotice", FVector3D(0.f, 220.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!_Notice)
	{
		return false;
	}

	_Dragon = LevelOwner->SpawnActor<BossDragon>("BossDragon", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!_Dragon)
	{
		return false;
	}

	BossSprite->AddNotify("LUCID_MOB_8880140.skill2", _PatternData.SpawnFrame, this, &BossDragonState::SpawnDragonPattern);

	return true;
}

void BossDragonState::Enter(Ptr<MonsterComponent> Monster)
{
	_CastStarted = false;

	_DragonStarted = false;

	if (_Notice)
	{
		_Notice->Show(L"루시드가 강력한 소환수를 소환했습니다!");
	}
}

Ptr<MonsterState> BossDragonState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster || !_Notice)
	{
		return nullptr;
	}

	if (!_Notice->IsFinished())
	{
		return nullptr;
	}

	if (!_CastStarted)
	{
		Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

		if (!MonsterOwner)
		{
			return nullptr;
		}

		Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

		if (!BossSprite)
		{
			return nullptr;
		}

		_CastStarted = true;

		BossSprite->ChangeAnimation("LUCID_MOB_8880140.skill2");

		return nullptr;
	}

	if (!_DragonStarted)
	{
		return nullptr;
	}

	return Lock(_IdleState);
}

bool BossDragonState::CanSelect() const
{
	return _Dragon && !_Dragon->IsEnable();
}

void BossDragonState::Reset()
{
	if (_Dragon)
	{
		_Dragon->SetPatternEnable(false);
	}

	if (_Notice)
	{
		_Notice->Hide();
	}

	_CastStarted = false;

	_DragonStarted = false;
}

void BossDragonState::SpawnDragonPattern()
{
	if (_DragonStarted)
	{
		return;
	}

	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return;
	}

	Ptr<MonsterStateMachine> StateMachine = Owner->GetStateMachine();

	if (!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	_DragonStarted = true;

	if (!_Dragon)
	{
		return;
	}

	Ptr<MonsterBlackBoard> MonsterBoard = Owner->GetBlackBoard();

	if (!MonsterBoard)
	{
		return;
	}

	Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return;
	}

	FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

	if (PatternArea.LeftBound >= PatternArea.RightBound)
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> SideDistribution(0, 1);

	bool LeftSide = SideDistribution(RandomEngine) == 0;

	float DragonX = LeftSide ? PatternArea.LeftBound + _PatternData.SpawnOffsetX : PatternArea.RightBound - _PatternData.SpawnOffsetX;

	FVector3D DragonPosition(DragonX, PatternArea.GroundY + _PatternData.DragonHeight, 0.f);

	FVector3D StartPosition = DragonPosition;

	StartPosition._y += _PatternData.SpawnHeight;

	_Dragon->Start(StartPosition, DragonPosition, LeftSide);
}

void BossDragonState::Destroy()
{
	if (_Notice)
	{
		_Notice->SetEnable(false);
	}

	if (_Dragon)
	{
		_Dragon->SetPatternEnable(false);
	}

	_Notice.reset();

	_Dragon.reset();

	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}