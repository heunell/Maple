#include "pch.h"
#include "BossTeleportState.h"
#include "Boss.h"
#include "BossBlackBoard.h"
#include "BossComponent.h"
#include "BossIdleState.h"
#include "BossPhase1Golem.h"
#include "BossTeleportEffect.h"
#include "Component/MovementComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/GameEngine.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "Object/Actor.h"
#include "World/World.h"
#include "World/Level.h"
#include <random>
#include <vector>

bool BossTeleportState::Init(Ptr<BossComponent> Owner)
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


	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return false;
	}

	_TeleportEffect = LevelOwner->SpawnActor<BossTeleportEffect>("BossTeleportEffect", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!_TeleportEffect)
	{
		return false;
	}

	BossSprite->AddNotify("LUCID_MOB_8880140.skill4", _PatternData.EffectStartFrame, this, &BossTeleportState::StartTeleportEffect);

	return true;
}

void BossTeleportState::Enter(Ptr<MonsterComponent> Monster)
{
	_EffectStarted = false;

	_Teleported = false;

	if (_TeleportEffect)
	{
		_TeleportEffect->SetEffectEnable(false);
	}

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

	BossSprite->ChangeAnimation("LUCID_MOB_8880140.skill4");
}

Ptr<MonsterState> BossTeleportState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster)
	{
		return nullptr;
	}

	if (!_Teleported)
	{
		return nullptr;
	}

	Ptr<BossComponent> BossController = Cast<MonsterComponent, BossComponent>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	return BossController->GetIdleState();
}

void BossTeleportState::StartTeleportEffect()
{
	if (_EffectStarted)
	{
		return;
	}

	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return;
	}

	Ptr<MonsterStateMachine> StateMachine = BossController->GetStateMachine();

	if (!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	_EffectStarted = true;

	Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

	if (!CurrentWorld)
	{
		_Teleported = true;

		return;
	}

	Ptr<Actor> Player = CurrentWorld->GetPlayer();

	if (!Player || !_TeleportEffect)
	{
		_Teleported = true;

		return;
	}

	_TeleportEffect->Start(This<BossTeleportState>(), Player);
}

void BossTeleportState::TeleportPlayer()
{
	if (_Teleported)
	{
		return;
	}

	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return;
	}

	Ptr<MonsterStateMachine> StateMachine = BossController->GetStateMachine();

	if (!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	_Teleported = true;

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

	Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

	if (!CurrentWorld)
	{
		return;
	}

	Ptr<Actor> Player = CurrentWorld->GetPlayer();

	if (!Player)
	{
		return;
	}

	Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");

	if (!Movement)
	{
		return;
	}

	std::vector<Ptr<BossPhase1Golem>> TeleportTargets;

	for (Weak<BossPhase1Golem>& GolemReference : BossBoard->ActivePhase1Golems)
	{
		Ptr<BossPhase1Golem> Golem = Lock(GolemReference);

		if (!Golem || !Golem->IsStanding())
		{
			continue;
		}

		TeleportTargets.push_back(Golem);
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	float DestinationX = 0.f;

	if (!TeleportTargets.empty())
	{
		std::uniform_int_distribution<int32> GolemDistribution(0, static_cast<int32>(TeleportTargets.size()) - 1);

		int32 GolemIndex = GolemDistribution(RandomEngine);

		DestinationX = TeleportTargets[GolemIndex]->GetWorldPosition()._x;
	}
	else
	{
		FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

		if (PatternArea.LeftBound >= PatternArea.RightBound)
		{
			return;
		}

		std::uniform_real_distribution<float> PositionDistribution(PatternArea.LeftBound, PatternArea.RightBound);

		DestinationX = PositionDistribution(RandomEngine);
	}

	FVector3D Destination = Player->GetWorldPosition();

	Destination._x = DestinationX;

	Destination._y = BossBoard->PatternArea.GroundY;

	Player->SetWorldPosition(Destination);

	Movement->ResetMovement();
}

void BossTeleportState::Reset()
{
	if (_TeleportEffect)
	{
		_TeleportEffect->SetEffectEnable(false);
	}

	_EffectStarted = false;

	_Teleported = false;
}

void BossTeleportState::Destroy()
{
	if (_TeleportEffect)
	{
		_TeleportEffect->SetEffectEnable(false);
	}

	_TeleportEffect.reset();

	_Owner.reset();

	MonsterState::Destroy();
}