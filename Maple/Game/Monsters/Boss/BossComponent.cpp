#include "pch.h"
#include "BossComponent.h"
#include "BossIdleState.h"
#include "BossBlackBoard.h"
#include "BossBladeState.h"
#include "BossFlowerState.h"
#include "BossMushroomState.h"
#include "BossPhase1GolemState.h"
#include "BossTeleportState.h"
#include "BossDragonState.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include <random>

bool BossComponent::Init(int32 Id, const std::string& Name, Ptr<Actor> Owner)
{
	if (!MonsterComponent::Init(Id, Name, Owner))
	{
		return false;
	}

	Ptr<BossBlackBoard> BlackBoard = New<BossBlackBoard>();

	if (!BlackBoard)
	{
		return false;
	}

	SetBlackBoard(BlackBoard);

	Ptr<MonsterStateMachine> StateMachine = GetStateMachine();

	if (!StateMachine)
	{
		return false;
	}

	_IdleState = New<BossIdleState>();

	if (!_IdleState)
	{
		return false;
	}

	Ptr<BossBladeState> BladeState = New<BossBladeState>();

	if (!BladeState)
	{
		return false;
	}

	FBossBladeStateData BladeData;

	BladeData.CastAnimation = "LUCID_MOB_8880140.skill1";

	BladeData.IdleAnimation = "LUCID_MOB_8880140.stand";

	BladeData.SpawnFrame = 34;

	BladeData.SpawnOffset = FVector3D(23.f, 288.5f, 0.f);

	if (!BladeState->Init(This<BossComponent>(), _IdleState, BladeData))
	{
		return false;
	}

	Ptr<BossFlowerState> FlowerState = New<BossFlowerState>();

	if (!FlowerState)
	{
		return false;
	}

	if (!FlowerState->Init(This<BossComponent>()))
	{
		return false;
	}

	Ptr<BossMushroomState> MushroomState = New<BossMushroomState>();
	
	if (!MushroomState)
	{
		return false;
	}

	if (!MushroomState->Init(This<BossComponent>()))
	{
		return false;
	}

	Ptr<BossPhase1GolemState> Phase1GolemState = New<BossPhase1GolemState>();

	if (!Phase1GolemState)
	{
		return false;
	}

	if (!Phase1GolemState->Init(This<BossComponent>()))
	{
		return false;
	}

	Ptr<BossTeleportState> TeleportState = New<BossTeleportState>();

	if (!TeleportState)
	{
		return false;
	}

	if (!TeleportState->Init(This<BossComponent>()))
	{
		return false;
	}

	Ptr<BossDragonState> DragonState = New<BossDragonState>();

	if (!DragonState)
	{
		return false;
	}

	if (!DragonState->Init(This<BossComponent>(), _IdleState))
	{
		return false;
	}

	if (!StateMachine->AddState(_IdleState))
	{
		return false;
	}

	if (!StateMachine->AddState(BladeState))
	{
		return false;
	}

	if (!StateMachine->AddState(FlowerState))
	{
		return false;
	}

	if (!StateMachine->AddState(MushroomState))
	{
		return false;
	}

	if(!StateMachine->AddState(Phase1GolemState))
	{
		return false;
	}

	if (!StateMachine->AddState(TeleportState))
	{
		return false;
	}

	if (!StateMachine->AddState(DragonState))
	{
		return false;
	}

	_PatternStates.push_back(BladeState);

	_PatternStates.push_back(FlowerState);

	_PatternStates.push_back(MushroomState);

	_PatternStates.push_back(Phase1GolemState);
	
	_PatternStates.push_back(TeleportState);

	_PatternStates.push_back(DragonState);

	TransitionState(_IdleState);

	return true;
}

void BossComponent::Destroy()
{
	MonsterComponent::Destroy();

	_PatternStates.clear();

	_IdleState.reset();
}

Ptr<MonsterState> BossComponent::SelectPatternState()
{
	std::vector<Ptr<MonsterState>> SelectableStates;

	for (Ptr<MonsterState>& PatternState : _PatternStates)
	{
		if (!PatternState || !PatternState->CanSelect())
		{
			continue;
		}

		SelectableStates.push_back(PatternState);
	}

	if (SelectableStates.empty())
	{
		return nullptr;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> PatternDistribution(0, static_cast<int32>(SelectableStates.size()) - 1);

	int32 PatternIndex = PatternDistribution(RandomEngine);

	return SelectableStates[PatternIndex];
}

Ptr<BossIdleState> BossComponent::GetIdleState() const
{
	return _IdleState;
}