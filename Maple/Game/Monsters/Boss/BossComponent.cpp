#include "pch.h"
#include "BossComponent.h"
#include "BossIdleState.h"
#include "BossBlackBoard.h"
#include "BossBladeState.h"
#include "BossFlowerState.h"
#include "BossMushroomState.h"
#include "BossPhase1GolemState.h"
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

	if (!BladeState->Init(This<BossComponent>()))
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

	_PatternStates.push_back(BladeState);

	_PatternStates.push_back(FlowerState);

	_PatternStates.push_back(MushroomState);

	_PatternStates.push_back(Phase1GolemState);
	
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