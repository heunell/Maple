#include "pch.h"
#include "BossPhase1GolemState.h"
#include "Boss.h"
#include "BossComponent.h"
#include "BossBlackBoard.h"
#include "BossIdleState.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "World/Level.h"

#include <functional>
#include <random>

bool BossPhase1GolemState::Init(Ptr<BossComponent> Owner)
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

	BossSprite->AddNotify("LUCID_MOB_8880140.skill3", _PatternData.SpawnFrame, this, &BossPhase1GolemState::SpawnGolemPattern);

	if (!_GolemPool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&BossPhase1GolemState::CreateGolem, this),
		std::bind_front(&BossPhase1GolemState::EnableGolem, this),
		std::bind_front(&BossPhase1GolemState::DisableGolem, this)))
	{
		return false;
	}

	return _GolemPool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<BossPhase1Golem> BossPhase1GolemState::CreateGolem()
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

	return LevelOwner->SpawnActor<BossPhase1Golem>("BossPhase1Golem", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void BossPhase1GolemState::EnableGolem(const Ptr<BossPhase1Golem>& Golem)
{
	Golem->SetPoolEnable(true);
}

void BossPhase1GolemState::DisableGolem(const Ptr<BossPhase1Golem>& Golem)
{
	Golem->SetPoolEnable(false);
}

void BossPhase1GolemState::Enter(Ptr<MonsterComponent> Monster)
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

Ptr<MonsterState> BossPhase1GolemState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
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

bool BossPhase1GolemState::CanSelect() const
{
	return _GolemPool.GetAvailableCount() >= static_cast<std::size_t>(_PatternData.SpawnCount);
}

void BossPhase1GolemState::SpawnGolemPattern()
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

	// Mushroom과 Golem이 skill3를 공유하므로 현재 선택된 State만 생성한다.
	if (!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	// 이후 생성에 실패해도 Boss가 PatternState에서 멈추지 않게 한다.
	_Spawned = true;

	Ptr<MonsterBase> MonsterOwner = BossController->GetMonster();

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

	if (_GolemPool.GetAvailableCount() < static_cast<std::size_t>(_PatternData.SpawnCount))
	{
		return;
	}

	FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

	if (_PatternData.FirstSlotIndex < 0 || _PatternData.LastSlotIndex < _PatternData.FirstSlotIndex)
	{
		return;
	}

	if (PatternArea.GroundSpawnPositions.size() <= static_cast<std::size_t>(_PatternData.LastSlotIndex))
	{
		return;
	}

	int32 SlotCount = _PatternData.LastSlotIndex - _PatternData.FirstSlotIndex + 1;

	if (SlotCount < _PatternData.SpawnCount)
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> SlotDistribution(_PatternData.FirstSlotIndex, _PatternData.LastSlotIndex);

	int32 FirstSlotIndex = SlotDistribution(RandomEngine);

	int32 SecondSlotIndex = SlotDistribution(RandomEngine);

	while (SecondSlotIndex == FirstSlotIndex)
	{
		SecondSlotIndex = SlotDistribution(RandomEngine);
	}

	FVector3D FirstGroundPosition = PatternArea.GroundSpawnPositions[FirstSlotIndex];

	FVector3D SecondGroundPosition = PatternArea.GroundSpawnPositions[SecondSlotIndex];

	FVector3D FirstStartPosition = FirstGroundPosition;

	FVector3D SecondStartPosition = SecondGroundPosition;

	float SpawnY = BossSprite->GetWorldPosition()._y;

	FirstStartPosition._y = SpawnY;

	SecondStartPosition._y = SpawnY;

	Ptr<BossPhase1Golem> FirstGolem = _GolemPool.Acquire();

	if (!FirstGolem)
	{
		return;
	}

	Ptr<BossPhase1Golem> SecondGolem = _GolemPool.Acquire();

	if (!SecondGolem)
	{
		_GolemPool.Release(FirstGolem);

		return;
	}

	FirstGolem->Start(This<BossPhase1GolemState>(), FirstStartPosition, FirstGroundPosition._y, _PatternData.FallDuration);

    SecondGolem->Start(This<BossPhase1GolemState>(), SecondStartPosition, SecondGroundPosition._y, _PatternData.FallDuration);

    BossBoard->ActivePhase1Golems.push_back(FirstGolem);

    BossBoard->ActivePhase1Golems.push_back(SecondGolem);
}

void BossPhase1GolemState::ReleaseGolem(Ptr<BossPhase1Golem> Golem)
{
	if (!Golem)
	{
		return;
	}

	Ptr<BossComponent> BossController = Lock(_Owner);

	if (BossController)
	{
		Ptr<MonsterBlackBoard> MonsterBoard = BossController->GetBlackBoard();

		Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

		if (BossBoard)
		{
			std::vector<Weak<BossPhase1Golem>>& ActiveGolems = BossBoard->ActivePhase1Golems;

			for (auto Iterator = ActiveGolems.begin(); Iterator != ActiveGolems.end();)
			{
				Ptr<BossPhase1Golem> ActiveGolem = Lock(*Iterator);

				if (!ActiveGolem || ActiveGolem == Golem)
				{
					Iterator = ActiveGolems.erase(Iterator);

					continue;
				}

				++Iterator;
			}
		}
	}

	_GolemPool.Release(Golem);
}

void BossPhase1GolemState::Reset()
{
	_GolemPool.ReleaseAll();

	_Spawned = false;
}

void BossPhase1GolemState::Destroy()
{
	_GolemPool.ReleaseAll();

	_Owner.reset();

	MonsterState::Destroy();
}