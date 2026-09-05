#include "pch.h"
#include "BossMushroomState.h"
#include "Boss.h"
#include "BossComponent.h"
#include "BossBlackBoard.h"
#include "BossIdleState.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "World/Level.h"
#include <functional>

bool BossMushroomState::Init(Ptr<BossComponent> Owner)
{
	if (!Owner)
	{
		return false;
	}

	_Owner = Owner;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<Boss> BossOwner = Cast<MonsterBase, Boss>(MonsterOwner);

	if (!BossOwner)
	{
		return false;
	}

	Ptr<SpriteComponent> BossSprite = BossOwner->GetBossSprite();

	if (!BossSprite)
	{
		return false;
	}

	BossSprite->AddNotify("LUCID_MOB_8880140.skill3", _PatternData.SpawnFrame, this, &BossMushroomState::SpawnMushroomPattern);

	if (!_MushroomPool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&BossMushroomState::CreateMushroom, this),
		std::bind_front(&BossMushroomState::EnableMushroom, this),
		std::bind_front(&BossMushroomState::DisableMushroom, this)))
	{
		return false;
	}

	return _MushroomPool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<BossMushroom> BossMushroomState::CreateMushroom()
{
	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = BossController->GetMonster();

	if (!MonsterOwner)
	{
		return nullptr;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return nullptr;
	}

	return LevelOwner->SpawnActor<BossMushroom>("BossMushroom", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void BossMushroomState::EnableMushroom(const Ptr<BossMushroom>& Mushroom)
{
	Mushroom->SetPoolEnable(true);
}

void BossMushroomState::DisableMushroom(const Ptr<BossMushroom>& Mushroom)
{
	Mushroom->SetPoolEnable(false);
}

void BossMushroomState::Enter(Ptr<MonsterComponent> Monster)
{
	_Spawned = false;

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

	BossSprite->ChangeAnimation("LUCID_MOB_8880140.skill3");
}

Ptr<MonsterState> BossMushroomState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster)
	{
		return nullptr;
	}

	Ptr<BossComponent> BossController = Cast<MonsterComponent, BossComponent>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	if (!_Spawned)
	{
		return nullptr;
	}

	return BossController->GetIdleState();
}

bool BossMushroomState::CanSelect() const
{
	return _MushroomPool.GetInUseCount() == 0;
}

void BossMushroomState::SpawnMushroomPattern()
{
	if (_Spawned)
	{
		return;
	}

	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return;
	}

	Ptr<MonsterStateMachine> StateMachine = BossController->GetStateMachine();

	if(!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	// Notify가 실행됐다는 뜻이며, 실패하더라도 State가 멈추지 않게 한다.
	_Spawned = true;
	
	Ptr<MonsterBase> MonsterOwner = BossController->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<MonsterBlackBoard> MonsterBoard = BossController->GetBlackBoard();

	if (!MonsterBoard)
	{
		return;
	}

	Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return;
	}

	if (_MushroomPool.GetAvailableCount() < static_cast<std::size_t>(_PatternData.PoolMaxCount))
	{
		return;
	}

	FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

	FVector3D BossPosition = MonsterOwner->GetWorldPosition();

	FVector3D LeftPosition(BossPosition._x - _PatternData.SpawnOffsetX, PatternArea.GroundY, BossPosition._z);

	FVector3D RightPosition(BossPosition._x + _PatternData.SpawnOffsetX, PatternArea.GroundY, BossPosition._z);

	Ptr<BossMushroom> LeftMushroom = _MushroomPool.Acquire();

	if (!LeftMushroom)
	{
		return;
	}

	Ptr<BossMushroom> RightMushroom = _MushroomPool.Acquire();

	if (!RightMushroom)
	{
		_MushroomPool.Release(LeftMushroom);

		return;
	}

	LeftMushroom->Start(This<BossMushroomState>(), LeftPosition, -1, PatternArea.LeftBound);

	RightMushroom->Start(This<BossMushroomState>(), RightPosition, 1, PatternArea.RightBound);
}

void BossMushroomState::ReleaseMushroom(Ptr<BossMushroom> Mushroom)
{
	if (!Mushroom)
	{
		return;
	}

	_MushroomPool.Release(Mushroom);
}

void BossMushroomState::Reset()
{
	_MushroomPool.ReleaseAll();

	_Spawned = false;
}

void BossMushroomState::Destroy()
{
	_MushroomPool.ReleaseAll();

	_Owner.reset();

	MonsterState::Destroy();
}